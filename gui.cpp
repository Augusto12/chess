#include <cstdio>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "moves.h"

ALLEGRO_BITMAP* whiteboard;
int w, h;
ALLEGRO_BITMAP* pawnImage,
              * rookImage,
              * knightImage,
              * bishopImage,
              * queenImage,
              * kingImage;


void draw_chess_pieces(int x, int y, char board[8][9], bool invert)
{
  bool isWhite;
  ALLEGRO_BITMAP* pieceImage;

  for (int i = 0; i < 8; ++i)
  {
    for (int j = 0; j < 8; ++j)
    {
      char piece = board[(invert ? 7 - i : i)][(invert ? 7 - j : j)];

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

void draw_chess_board(int x, int y, bool invert)
{
  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
      al_draw_tinted_bitmap(whiteboard,
                            ((i + j) % 2 == 0) ?
                              al_map_rgb(0xff, 0xff, 0xff) :
                              al_map_rgb(0x77, 0x77, 0x77),
                            x + (invert ? 7 - j : j) * w, y + (invert ? 7 - i : i) * h,
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

  // Networking
  unsigned short port = 30000;
  bool server;

  int sockfd, serverfd;
  if (argc == 1)
  {
    server = true;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(serverfd,
             (const sockaddr*) &addr,
             sizeof(sockaddr_in)) < 0)
    {
      perror("Failed to bind socket");
      return -1;
    }

    // IP Address
    ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
    ioctl(serverfd, SIOCGIFADDR, &ifr);
    printf("IP Address: %s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    listen(serverfd, 1);
    printf("Waiting for player!\n");

    sockaddr_in connaddr;
    socklen_t connlen = sizeof(connaddr);
    sockfd = accept(serverfd,
                    (sockaddr*) &connaddr,
                    &connlen);
    if (sockfd < 0)
    {
      perror("Failed to accept connection");
      return -1;
    }

    printf("Connected: %s\n", inet_ntoa(connaddr.sin_addr));
  }
  else if (argc == 2)
  {
    server = false;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_aton(argv[1], &addr.sin_addr);
    addr.sin_port = htons(port);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sockfd, (sockaddr*) &addr, sizeof(addr)) < 0)
    {
      //printf("ERROR connecting!\n");
      perror("ERROR connecting");
      return -1;
    }
  }

  if (fcntl(sockfd, F_SETFL, O_NONBLOCK, 1) == -1)
  {
    printf("Failed to set non-blocking\n");
    return -1;
  }

  // Main loop
  Cor corjogador = (server ? BRANCO : PRETO);
  int jogada = 0;
  bool selecting = true;
  int sl, sc;

  bool sair = false;
  while (!sair)
  {
    Cor corjogada = (jogada % 2 == 0 ? BRANCO : PRETO);
    bool acted = false;

    // 
    char buffer[256];
    if (corjogada != corjogador)
    {
      if (read(sockfd, buffer, 256) < 0)
      {
        if (errno != EAGAIN)
        {
          perror("ERROR reading from socket");
          break;
        }
      }
      else
      {
        int l, c;
        //printf("buffer: %s\n", buffer);
        sscanf(buffer, "%d %d %d %d", &sl, &sc, &l, &c);
        printf("Opponent movement: %c%d %c%d\n", 'a' + sc, (8 - sl), 'a' + c, (8 - l));
        if (sl >= 0 && sl < 8 && sc >= 0 && sc < 8 &&
            l >= 0 && l < 8 && c >= 0 && c < 8)
        {
          escolher_destino(board, sl, sc, l, c);
          jogada++;
          //continue;
        }
        else
        {
          printf("Invalid opponent movement!\n");
        }
      }
    }

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
          {
            if (corjogada != corjogador) break;
            if (event.mouse.button != 1) break;
            if (acted) break;

            int c = event.mouse.x / w,
                l = event.mouse.y / h;
            if (!server)
            {
              c = 7 - c;
              l = 7 - l;
            }

            if (selecting)
            {
              if (selecionar_peca(board, l, c, corjogada))
              {
                sl = l;
                sc = c;
                acted = true;
                printf("Selected piece: %c%d\n", 'a' + sc, (8 - sl));
              }
              else
                printf("Invalid position!\n");
            }
            else
            {
              if (l == sl && c == sc)
              {
                printf("Deselected piece!\n");
                acted = true;
              }
              else if (escolher_destino(board, sl, sc, l, c))
              {
                sprintf(buffer, "%d %d %d %d", sl, sc, l, c);
                write(sockfd, buffer, strlen(buffer) + 1);
                //printf("buffer: (%s)\n", buffer);

                printf("Movement: %c%d %c%d\n", 'a' + sc, (8 - sl), 'a' + c, (8 - l));
                acted = true;
                jogada++;
              }
              else
                printf("Invalid destination!\n");
            }
          }
        break;

        default:
        break;
      }
    }

    if (acted)
      selecting = !selecting;

    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Render
    draw_chess_board (0, 0, !server);
    draw_chess_pieces(0, 0, board, !server);

    al_flip_display();
  }

  al_destroy_display(display);
  al_destroy_event_queue(eventQueue);

  if (sockfd >= 0) close(sockfd);
  if (serverfd >= 0) close(serverfd);

  return 0;
}
