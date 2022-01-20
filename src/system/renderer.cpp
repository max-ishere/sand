#include "sand/core/noise.hpp"
#include "sand/core/random.hpp"
#include "sand/core/timer.hpp"
#include "sand/dependencies/imgui/imgui_impl_sdl.hpp"
#include "sand/dependencies/imgui/imgui_impl_sdlrenderer.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <box2d/b2_body.h>
#include <box2d/b2_math.h>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <imgui.h>
#include <iostream>
#include <sand/component/renderer_data.hpp>
#include <sand/system/renderer.hpp>
#include <sand/system/sprite_data.hpp>

void Renderer::operator()(entt::registry &registry) {
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);

  static_assert(Renderer::position_to_pixels % 2 == 0,
                "For easier screen centering math pixel scale should be even");

  int width = 0, height = 0;
  SDL_GetWindowSize(window, &width, &height);

  int x_offset_camera = static_cast<int>(
      (camera_data.x - floor(camera_data.x)) * Renderer::position_to_pixels);
  int y_offset_camera =
      static_cast<int>((1.f - camera_data.y + floor(camera_data.y)) *
                       Renderer::position_to_pixels);

  this->RenderGround(sprite_data, width, height, x_offset_camera,
                     y_offset_camera);

  this->RenderEntites(registry, width, height);
  this->RenderDebug();
  this->RenderImGui(registry);

  ImGui::Render();
  ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

  SDL_RenderPresent(renderer);
}

namespace {
void SDLassert(void *ptr) {
#ifdef DEBUG
  if (ptr == nullptr) {
    std::cerr << "SDL error: " << SDL_GetError() << "\n";
  }
#endif
}
} // namespace

Renderer::Renderer() {
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("Sand Game", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 640, 480,
                            SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  SDLassert(window);

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDLassert(renderer);

  SDL_Surface *temp_surface = SDL_LoadBMP("assets/tilemap.bmp");
  SDLassert(temp_surface);
  sprite_data.tilemap = SDL_CreateTextureFromSurface(renderer, temp_surface);
  SDL_FreeSurface(temp_surface);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForSDLRenderer(window);
  ImGui_ImplSDLRenderer_Init(renderer);
}

Renderer::~Renderer() {
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Renderer::RenderGround(const SpriteData &sprite_data, const int width,
                            const int height, const int x_offset_camera,
                            const int y_offset_camera) {
  const int grid_width = width / Renderer::position_to_pixels + 4,
            grid_height = height / Renderer::position_to_pixels + 4;

  int x_offset_edge =
      Renderer::position_to_pixels -
      static_cast<int>(round((width - Renderer::position_to_pixels) / 2)) %
          Renderer::position_to_pixels;
  int y_offset_edge =
      Renderer::position_to_pixels -
      static_cast<int>(round((height - Renderer::position_to_pixels) / 2)) %
          Renderer::position_to_pixels;

  for (auto i = 0u; i < grid_width; i++)
    for (auto j = 0u; j < grid_height; j++) {
      constexpr SDL_Rect grass{0, 16, 16, 16};

      SDL_Rect sprite_position{
          .x = static_cast<int>((i * Renderer::position_to_pixels) -
                                x_offset_edge - x_offset_camera),
          .y = static_cast<int>((j * Renderer::position_to_pixels) -
                                y_offset_edge - y_offset_camera),
          .w = Renderer::position_to_pixels,
          .h = Renderer::position_to_pixels,
      };

      SDL_RenderCopy(renderer, sprite_data.tilemap, &grass, &sprite_position);
    }
}

void Renderer::RenderEntites(entt::registry &registry,
                             const std::integral auto width,
                             const std::integral auto height) {
  int center_x = round((width - position_to_pixels) / 2),
      center_y = round((height - position_to_pixels) / 2);

  registry.view<RendererData, b2Body *>().use<RendererData>().each(
      [this, center_x, center_y](const auto &renderer_data, const auto &body) {
        b2Vec2 position = body->GetPosition();
        SDL_Rect sprite_position{
            .x = static_cast<int>(
                round((position.x - camera_data.x + renderer_data.x_offset) *
                          position_to_pixels +
                      center_x)),
            .y = static_cast<int>(
                round(-(position.y - camera_data.y + renderer_data.y_offset) *
                          position_to_pixels +
                      center_y)),

            .w = position_to_pixels,
            .h = position_to_pixels,
        };
        if (!renderer_data.sprite) {
          SDL_RenderDrawRect(renderer, &sprite_position);
        } else {
          SDL_Rect sprite = sprite_data(renderer_data.sprite_id);
          SDL_RenderCopy(renderer, sprite_data.tilemap, &sprite,
                         &sprite_position);
        }
      });
}

void Renderer::RenderDebug() {
  const int SIZE_X = 300, SIZE_Y = 300;
  SDL_Texture *texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, SIZE_X, SIZE_Y);
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer);

  const auto SCALE = 20;
  static auto noise_map =
      ScaleArray<uint32_t, SIZE_X / SCALE, SIZE_Y / SCALE, SCALE>(
          CropArray<uint32_t, SIZE_X, SIZE_Y, SIZE_X / SCALE>(
              RandomNoiseMap<uint32_t, SIZE_X, SIZE_Y>(FastRandom())));

  ImGui::Begin("Perlin noise thing");
  static bool blur = false;
  static auto display_map = noise_map;
  bool update = false;
  update = ImGui::Checkbox("Enable blur", &blur) ? true : update;
  Timer timer;
  if (blur) {

    static int averaging_radius = 1;
    update = ImGui::SliderInt("Averaging radius", &averaging_radius, 1, 10)
                 ? true
                 : update;
    static int iterations = 1;
    update = ImGui::SliderInt("Iterations", &iterations, 1, 10) ? true : update;
    if (update) {
      display_map = noise_map;
      timer.start();
      for (auto i = 0; i < iterations; i++)
        display_map = BlurArray(display_map, averaging_radius + i);
      // timer.stop();
      update = false;
    }
  } else
    display_map = noise_map;
  ImGui::Text("%f", timer.elapsedMilliseconds());
  for (auto x = 0u; x < SIZE_X; x++)
    for (auto y = 0u; y < SIZE_Y; y++) {
      SDL_SetRenderDrawColor(renderer, display_map[x][y] + 10,
                             display_map[x][y] + 2, display_map[x][y], 0xff);
      SDL_Rect rect{.x = (int)x, .y = (int)y, .w = 1, .h = 1};
      SDL_RenderFillRect(renderer, &rect);
    }

  SDL_SetRenderTarget(renderer, NULL);
  auto pos = SDL_Rect{.x = 15, .y = 15, .w = SIZE_X * 2, .h = SIZE_Y * 2};
  SDL_RenderCopy(renderer, texture, NULL, &pos);
  SDL_DestroyTexture(texture);

  ImGui::End();
}

void Renderer::RenderImGui(entt::registry &registry) {

  // ImGui code here
}
