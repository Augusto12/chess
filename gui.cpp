#include <cstdio>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>


ALLEGRO_BITMAP* whiteboard;
int w, h;
ALLEGRO_BITMAP* pawnImage,
              * rookImage,
              * knightImage,
              * bishopImage,
              * queenImage,
              * kingImage;


void draw_chess_pieces(int x, int y, char board[8][9])
{
  bool isWhite;
  ALLEGRO_BITMAP* pieceImage;

  for (int i = 0; i < 8; ++i)
  {
    for (int j = 0; j < 8; ++j)
    {
      char piece = board[i][j];

      if (piece == ' ')
        continue;

      if (piece == 'p' || piece == 'r' ||
          piece == 'n' || piece == 'b' ||
          piece == 'q' || piece == 'k')
        isWhite = true;
      else
        isWhite = false;

      if (piece == 'p' || piece == 'P')
        pieceImage = pawnImage;
      else if (piece == 'r' || piece == 'R')
        pieceImage = rookImage;
      else if (piece == 'n' || piece == 'N')
        pieceImage = knightImage;
      else if (piece == 'b' || piece == 'B')
        pieceImage = bishopImage;
      else if (piece == 'q' || piece == 'Q')
        pieceImage = queenImage;
      else if (piece == 'k' || piece == 'K')
        pieceImage = kingImage;

      al_draw_tinted_bitmap(pieceImage,
                            isWhite ?
                              al_map_rgb(0xff, 0xff, 0xff) :
                              al_map_rgb(0x44, 0x44, 0x44),
                            x + j * w, y + i * h,
                            0);
    }
  }
}

void draw_chess_board(int x, int y)
{
  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
      al_draw_tinted_bitmap(whiteboard,
                            ((i + j) % 2 == 0) ?
                              al_map_rgb(0xff, 0xff, 0xff) :
                              al_map_rgb(0x77, 0x77, 0x77),
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
  whiteboard = al_load_bitmap("imgs/white.png");
  if (!whiteboard)
  {
    printf("Failed to load whiteImage!\n");
    return -1;
  }
  w = al_get_bitmap_width (whiteboard),
  h = al_get_bitmap_height(whiteboard);

  pawnImage   = al_load_bitmap("imgs/pawn.png");
  rookImage   = al_load_bitmap("imgs/rook.png");
  knightImage = al_load_bitmap("imgs/knight.png");
  bishopImage = al_load_bitmap("imgs/bishop.png");
  queenImage  = al_load_bitmap("imgs/queen.png");
  kingImage   = al_load_bitmap("imgs/king.png");
  if (!pawnImage || !rookImage || !knightImage || !bishopImage ||
      !queenImage || !kingImage)
  {
    printf("Failed to load image\n");
    return -1;
  }

  // Chess board
  char board[8][9] = {"RNBQKBNR",
                      "PPPPPPPP",
                      "        ",
                      "        ",
                      "        ",
                      "        ",
                      "pppppppp",
                      "rnbqkbnr"};

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
    draw_chess_board (0, 0);
    draw_chess_pieces(0, 0, board);

    al_flip_display();
  }

  al_destroy_display(display);
  al_destroy_event_queue(eventQueue);

  return 0;
}
