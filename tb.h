#include <SDL2/SDL.h>

class TB
{
protected:

    SDL_Surface*        screen_surface;
    SDL_Window*         sdl_window;
    SDL_Renderer*       sdl_renderer;
    SDL_PixelFormat*    sdl_pixel_format;
    SDL_Texture*        sdl_screen_texture;
    SDL_Event           evt;
    Uint32*             screen_buffer;

    // Обработка фрейма
    int width, height, scale, frame_length, pticks, htop, rgb3bit;
    int savevideo = 0, frame_ticker = 0;
    int x, y, _hs, _vs;

    // Модули
    Vvideo*     video;

    FILE* ppm = NULL;

public:

    TB(int argc, char** argv)
    {
        x   = 0;
        y   = 0;
        _hs = 1;
        _vs = 0;

        int i = 1;
        int hs;

        scale        = 2;               // Удвоение пикселей
        width        = 640;             // Ширина экрана
        height       = 400;             // Высота экрана
        htop         = 35;
        frame_length = (1000/20);       // 20 FPS
        pticks       = 0;
        rgb3bit      = 0;

        // Разбор параметров
        while (i < argc) {

            if (argv[i][0] == '-') {

                switch (argv[i][1]) {

                    // 640x480
                    case 'H': height = 480; htop = 33; break;

                    // RGB однобитные
                    case 'L': rgb3bit = 1; break;

                    // Сохранить в PPM видео
                    case 'v': savevideo = 1; ppm = fopen("record.ppm", "wb"); break;
                    case 'V': savevideo = 2; ppm = stdout; break;
                }
            }

            i++;
        }

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
            exit(1);
        }

        SDL_ClearError();
        sdl_window          = SDL_CreateWindow("DEMO VERSION", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              scale * width, scale * height, SDL_WINDOW_SHOWN);
        sdl_renderer        = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_PRESENTVSYNC);
        screen_buffer       = (Uint32*) malloc(width * height * sizeof(Uint32));
        sdl_screen_texture  = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING,
                              width, height);
        SDL_SetTextureBlendMode(sdl_screen_texture, SDL_BLENDMODE_NONE);

        // Запуск модулей
        video = new Vvideo;
    }

    // 1 Такт
    void tick()
    {
        video->clock = 0; video->eval();
        video->clock = 1; video->eval();

        int mulcl = rgb3bit ? 128 : 16;
        vga(video->hs, video->vs, video->r*mulcl*65536 + video->g*mulcl*256 + video->b*mulcl);

        if (++frame_ticker == 25000000/60) { saveframe(); frame_ticker = 0; }
    }

    // Основной цикл работы
    int main()
    {
        SDL_Rect dstRect;

        dstRect.x = 0;
        dstRect.y = 0;
        dstRect.w = scale * width;
        dstRect.h = scale * height;

        for (;;) {

            int count = 0;

            // Прием событий
            while (SDL_PollEvent(& evt)) {

                // Событие выхода
                switch (evt.type) { case SDL_QUIT: return 0; }
            }

            // Выполнение фрейма длиной <frame_length>: ms.
            do { for (int i = 0; i < 4096; i++) { tick(); count++; } }
            while (SDL_GetTicks() - pticks < frame_length);

            // Обновление экрана
            pticks = SDL_GetTicks();

            SDL_UpdateTexture       (sdl_screen_texture, NULL, screen_buffer, width * sizeof(Uint32));
            SDL_SetRenderDrawColor  (sdl_renderer, 0, 0, 0, 0);
            SDL_RenderClear         (sdl_renderer);
            SDL_RenderCopy          (sdl_renderer, sdl_screen_texture, NULL, & dstRect);
            SDL_RenderPresent       (sdl_renderer);
            SDL_Delay(1);

            return 1;
        }
    }

    // Убрать окно из памяти
    int destroy()
    {
        free(screen_buffer);
        SDL_DestroyTexture(sdl_screen_texture);
        SDL_FreeFormat(sdl_pixel_format);
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return 0;
    }

    // Установка точки
    void pset(int x, int y, Uint32 cl)
    {
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return;
        }

        screen_buffer[width*y + x] = cl;
    }

    // Отслеживание сигнала RGB по HS/VS
    void vga(int hs, int vs, int cl) {

        if (hs) x++;
        if (_hs == 1 && hs == 0) { x = 0; y++; }
        if (_vs == 0 && vs == 1) { y = 0; }
        _hs = hs;
        _vs = vs;
        pset(x-(48+2), y-htop, cl);
    }


    // Сохранение фрейма
    void saveframe()
    {
        if (savevideo == 0) {
            return;
        }

        if (ppm) {

            fprintf(ppm, "P6\n# APPLICATION\n%d %d\n255\n", width, height);
            for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++) {

                int cl = screen_buffer[y*width + x];
                int vl = ((cl >> 16) & 255) + (cl & 0xFF00) + ((cl&255)<<16);
                fwrite(&vl, 1, 3, ppm);
            }
        }
    }
};
