#include <cstdio>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

ALLEGRO_BITMAP* whiteImage;
int w, h;
ALLEGRO_BITMAP* pawnImage;

char board[8][9] = {"RNBQKBNR",
                    "PPPPPPPP",
                    "        ",
                    "        ",
                    "        ",
                    "        ",
                    "pppppppp",
                    "rnbqkbnr"};

void draw_chess_pieces(int x, int y)
{
  bool isWhite;
  ALLEGRO_BITMAP* pieceImage;

  for (int i = 0; i < 8; ++i)
  {
    for (int j = 0; j < 8; ++j)
    {
      char piece = board[j][i];

      if (piece == ' ')
        continue;

      if (piece == 'p' || piece == 'r' ||
          piece == 'n' || piece == 'b' ||
          piece == 'q' || piece == 'k')
        isWhite = true;
      else
        isWhite = false;

      //if (piece == 'p' || piece == 'P')
        pieceImage = pawnImage;
      //

      al_draw_tinted_bitmap(pieceImage,
                            isWhite ?
                              al_map_rgb(0xff, 0xff, 0xff) :
                              al_map_rgb(0x22, 0x22, 0x22),
                            x + i * w, y + j * h,
                            0);
    }
  }
}

void draw_chess_board(int x, int y)
{
  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
      al_draw_tinted_bitmap(whiteImage,
                            ((i + j) % 2 == 1) ?
                              al_map_rgb(0x77, 0x77, 0x77) :
                              al_map_rgb(0xff, 0xff, 0xff),
                            x + j * w, y + i * h,
                            0);
}

int main(int argc, char** argv)
{
  // Initialize Allegro
  al_init();
  al_install_keyboard();
  al_install_mouse();

  // Create window
  ALLEGRO_DISPLAY* display = al_create_display(512, 512);

  // Create event queue
  ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
  al_register_event_source(eventQueue, al_get_display_event_source(display));
  al_register_event_source(eventQueue, al_get_mouse_event_source());

  // Bitmaps
  al_init_image_addon();
  whiteImage = al_load_bitmap("imgs/white.png");
  if (!whiteImage)
  {
    printf("Failed to load whiteImage!\n");
    return -1;
  }
  w = al_get_bitmap_width (whiteImage),
  h = al_get_bitmap_height(whiteImage);

  pawnImage = al_load_bitmap("imgs/pawn.png");
  if (!pawnImage)
  {
    printf("Failed to load pawnImage\n");
    return -1;
  }

  // Main loop
  bool sair = false;
  while (!sair)
  {
    ALLEGRO_EVENT event;
    while (!al_is_event_queue_empty(eventQueue))
    {
      al_get_next_event(eventQueue, &event);

      switch (event.type)
      {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
          sair = true;
        break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
          printf("mouse down: %d %d %u\n",
                 event.mouse.x / w, event.mouse.y / h,
                 event.mouse.button);
        break;

        default:
        break;
      }
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Render
    draw_chess_board(0, 0);
    draw_chess_pieces(0, 0);

    al_flip_display();
  }

  al_destroy_display(display);
  al_destroy_event_queue(eventQueue);

  return 0;
}
