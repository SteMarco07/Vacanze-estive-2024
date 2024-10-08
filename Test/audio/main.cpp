#include "include/raylib.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct Ball {
    int raggio;
    float x;
    float y ;
    bool rimbalzo;
};

struct Player {
    int punteggio;
    Rectangle barretta;
};


struct Speeds {
    float v0;
    float v_ball_x;
    float v_ball_y;
    float v_barretta_1;
    float v_barretta_2;
};


struct Game {
    const int screenWidth;
    const int screenHeight;
    int stato;
    int stato_successivo;
    int gioco;
    float h_iniz;
    int tempo;
    int tempo_partita;
    int incremento;
    int inserimento_nome;
    bool pausa;
    int pagina_tasti;
};

struct String {
    std::vector<std::vector<std::string>> frasi;
    std::vector<float> dim_stringhe;
};

struct RGB {
    std::vector<unsigned char> canali_colore;
    Color colore;
    bool cresci;
};

struct Colors {
    RGB rgb_1;
    RGB rgb_2;
    bool RGB_Yes_No;
    std::vector<Color> colore_elementi;
    std::vector<int> scelta_elemento;
    std::vector<Color> colori_partenza;
    std::vector<Color> colori_custom;
    Color colore_temporaneo;
    int colorare;
    int elemento_da_colorare;
};

struct Triangle {
    Vector2 punto_1;
    Vector2 punto_2;
    Vector2 punto_3;
};

struct Cursor {
    Texture cliccato;
    Texture non_cliccato;
};

float valore_scalato(const Game &game, float valore, bool x ) {
    if (x) {
        return valore * game.screenWidth / 1920;
    } else {
        return valore * game.screenHeight / 1080;
    }
}


void stampa_elenco (const Game &game, const String &stringhe, const Colors &colori,int stato, int inizio, int fine) {
    int pos = game.screenHeight/2 - valore_scalato(game, 80, false) - valore_scalato(game, 100, false)*((fine-inizio)/2-1);
    Color colore = colori.colore_elementi.at(colori.scelta_elemento.at(2));
    for (int i = inizio; i < fine; ++i) {
        DrawText(stringhe.frasi.at(stato).at(i).c_str(), game.screenWidth * 0.02, pos, stringhe.dim_stringhe.at(2), colore);
        pos += valore_scalato(game, 100, false);
    }
}

int interazione_elenco (const Game &game, const String &stringhe,int stato, int inizio, int fine) {
    int pos = game.screenHeight/2 - valore_scalato(game, 80, false) - valore_scalato(game, 100, false)*((fine-inizio)/2-1);
    for (int i = inizio; i < fine; ++i) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {float(game.screenWidth*0.02), float(pos),(float)
                (MeasureText(stringhe.frasi.at(stato).at(i).c_str(), stringhe.dim_stringhe.at(2))), stringhe.dim_stringhe.at(2)})) {
            return i;
        }
        pos += valore_scalato(game, 100, false);
    }
    return 0;
}

void sposta_tra_stati (const Triangle &triangolo, Game &game, int stato_a_cui_andare, bool caricamento) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointTriangle(GetMousePosition(), triangolo.punto_1, triangolo.punto_2, triangolo.punto_3)) {
        if (caricamento) {
            game.stato = 1;
            game.stato_successivo = stato_a_cui_andare;
        } else {
            game.stato = stato_a_cui_andare;
        }
    }
}

void dis_scelta_elemento_colorare_stato_21 (const Game &game, const String &stringhe, const Colors &colori){
    Color colore = colori.colore_elementi.at(colori.scelta_elemento.at(2));
    DrawText(stringhe.frasi.at(21).at(0).c_str(), (GetScreenWidth() - MeasureText(stringhe.frasi.at(21).at(0).c_str(), stringhe.dim_stringhe.at(1)))/2, GetScreenHeight()*0.02, stringhe.dim_stringhe.at(1),colore);
    stampa_elenco(game, stringhe, colori, 21, 1, 5);
}

void dis_colori(const Game &game, const String &stringhe, const Colors &colori){
    int a = 0;
    int b = 0;
    std::string numero;
    DrawText( stringhe.frasi.at(22).at(0).c_str(), (game.screenWidth - MeasureText(stringhe.frasi.at(22).at(0).c_str(),stringhe.dim_stringhe.at(1)))/2,game.screenHeight*0.02,stringhe.dim_stringhe.at(1),colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    for (int i = 0; i <= colori.colore_elementi.size() + 1; ++i) {
        if (i >= 4)
            b = GetScreenHeight() / 2 - 50;

        if (i==4)
            a=0;
        DrawRectangle(game.screenWidth / 18 + a, GetScreenHeight() / 9 + 100 + b, game.screenWidth / 7.2, game.screenHeight / 3.6, DARKGRAY);
        if (i <= colori.colore_elementi.size()) {
            DrawRectangle(game.screenWidth / 18 + 5 + a, game.screenHeight / 9 + 105 + b, game.screenWidth / 7.2-10, game.screenHeight / 3.6-10, colori.colori_partenza.at(i));
        } else {
            DrawRectangle(game.screenWidth / 18 + 5 + a, game.screenHeight / 9 + 105 + b, game.screenWidth / 7.2-10, game.screenHeight / 3.6-10, colori.colori_custom.at(colori.elemento_da_colorare));
        }
        a += game.screenWidth / 4;
    }
    DrawRectangle(game.screenWidth / 18 + a, game.screenHeight / 9 + 100 + b, game.screenWidth / 7.2, game.screenHeight / 3.6, DARKGRAY);
    a=0;
    b=0;
    DrawText("1",game.screenWidth / 18 + 20,game.screenHeight / 9 + 105, 100, WHITE);
    for (int i = 2 ; i <= colori.colori_partenza.size() + 1 ; i++ ){
        a += game.screenWidth/4;
        if ( i == 5){
            a=0;
            b=game.screenHeight / 2 - 50;
        }
        if ( i <= colori.colori_partenza.size()) {
            numero = std::to_string(i);
            DrawText(numero.c_str(),game.screenWidth / 18 + 20 + a,game.screenHeight / 9 + 105 + b, 100, BLACK);
        } else {
            DrawText("custom",game.screenWidth / 18 + 20 + a,game.screenHeight / 9 + 105 + b, 70, BLACK);
        }
    }
}

void selezione_colore_stato_22 (Game &game, Colors &colori) {
    int a = 0, b = 0;
    float lato = game.screenWidth / 7.2-10, x, y;
    for (int i = 0; i <= colori.colori_partenza.size(); ++i) {
        if (i >= 4)
            b = GetScreenHeight() / 2 - 50;

        if (i==4)
            a=0;

        x = game.screenWidth / 18 + 5 + a, y = game.screenHeight / 9 + 105 + b;
        if (i < colori.colori_partenza.size()) {
            if (CheckCollisionPointRec(GetMousePosition(), {x, y, lato, lato}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                game.stato = 1;
                game.stato_successivo = 21;
                colori.colore_temporaneo = colori.colori_partenza.at(i);
                colori.colorare = 2;
            }
        } else{
            if (CheckCollisionPointRec(GetMousePosition(), {x, y, lato, lato}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                game.stato = 23;
            }
        }

        a += game.screenWidth / 4;
    }

}

void agg_pos_barretta_x_1(const Game &game, const Speeds &velocita, std::vector<Player> &giocatori) {
    float l = giocatori.at(0).barretta.width;
    if (IsKeyDown(KEY_A) && giocatori.at(0).barretta.x - l/2> 0) {
        giocatori.at(0).barretta.x -= velocita.v_barretta_1;
    } else if (IsKeyDown(KEY_D) && giocatori.at(0).barretta.x + giocatori.at(0).barretta.width < game.screenWidth/3) {
        giocatori.at(0).barretta.x += velocita.v_barretta_1;
    }
}

void agg_pos_barretta_x_2(const Game &game, const Speeds &velocita, std::vector<Player> &giocatori) {
    float l = giocatori.at(1).barretta.width;
    if (IsKeyDown(KEY_LEFT) && giocatori.at(1).barretta.x > game.screenWidth*2/3) {
        giocatori.at(1).barretta.x -= velocita.v_barretta_2;
    } else if (IsKeyDown(KEY_RIGHT) && giocatori.at(1).barretta.x + l < game.screenWidth) {
        giocatori.at(1).barretta.x += velocita.v_barretta_2;
    }
}

void agg_pos_barretta_y_1(const Speeds &velocita, std::vector<Player> &giocatori) {
    if (IsKeyDown(KEY_W) && giocatori.at(0).barretta.y > 0) {
        giocatori.at(0).barretta.y -= velocita.v_barretta_1;
    } else if (IsKeyDown(KEY_S) && giocatori.at(0).barretta.y + giocatori.at(0).barretta.height < GetScreenHeight()) {
        giocatori.at(0).barretta.y += velocita.v_barretta_2;
    }
}

void agg_pos_barretta_y_2(const Speeds &velocita, std::vector<Player> &giocatori) {
    if (IsKeyDown(KEY_UP) && giocatori.at(1).barretta.y > 0) {
        giocatori.at(1).barretta.y -= velocita.v_barretta_2;
    } else if (IsKeyDown(KEY_DOWN) && giocatori.at(1).barretta.y + giocatori.at(1).barretta.height < GetScreenHeight()) {
        giocatori.at(1).barretta.y += velocita.v_barretta_2;
    }
}

void agg_pos_pallina_y( Ball &pallina, Speeds &velocita){
    if ( pallina.y - pallina.raggio <= 0 ) {
        velocita.v_ball_y = abs(velocita.v_ball_y);
    } else if (pallina.y + pallina.raggio >= GetScreenHeight()) {
        velocita.v_ball_y = -velocita.v_ball_y;
    }
    pallina.y += velocita.v_ball_y;
}

void reset_v (const Game &game, Speeds &velocita){
    if (velocita.v_ball_y > 0) {
        velocita.v_ball_y = velocita.v0 + game.incremento;
    } else {
        velocita.v_ball_y = -velocita.v0 - game.incremento;
    }
}

void agg_pos_pallina_x(Game &game ,Ball &pallina,std::vector<Player> &giocatori, Speeds &velocita){
    float h = giocatori.at(0).barretta.height;
    if  (CheckCollisionCircleRec ({pallina.x, pallina.y}, pallina.raggio, giocatori.at(0).barretta) && pallina.rimbalzo) {
        velocita.v_ball_x = -velocita.v_ball_x;
        pallina.rimbalzo = false;
        pallina.x += 10;
        reset_v(game, velocita);
        velocita.v_ball_y *= (pallina.y - (giocatori.at(0).barretta.y+h/2))/(h/2);

    } else if (CheckCollisionCircleRec({pallina.x, pallina.y}, pallina.raggio, giocatori.at(1).barretta) && !pallina.rimbalzo) {
        velocita.v_ball_x = -velocita.v_ball_x;
        pallina.rimbalzo = true;
        pallina.x-= 10;
        reset_v(game, velocita);
        velocita.v_ball_y *= (pallina.y - (giocatori.at(1).barretta.y+h/2))/(h/2);
    }
    pallina.x += velocita.v_ball_x;
}

void vi_X (Speeds &velocita){
    if (GetRandomValue(1,2) == 1){
        velocita.v_ball_x = -velocita.v0;
    } else {
        velocita.v_ball_x = velocita.v0;
    }
}

void vi_y (Speeds &velocita){
    if (GetRandomValue(1,2) == 1){
        velocita.v_ball_y = -velocita.v0;
    } else {
        velocita.v_ball_y = velocita.v0;
    }
}

void dis_punteggio (int p, float pos, const Colors &colori) {
    std::string punteggio;
    punteggio = std::to_string(p);
    DrawText(punteggio.c_str(), pos, 30, 40, colori.colore_elementi.at(colori.scelta_elemento.at(2)));

}

void interruzioni (Game &game, const std::vector<Player> &giocatori, const Colors &colori, const String &stringhe) {
    int screenWidth = game.screenWidth, screenHeight = game.screenHeight;
    float dimensione_1 = stringhe.dim_stringhe.at(2), dimensione_2 = stringhe.dim_stringhe.at(3);
    Color colore = colori.colore_elementi.at(colori.scelta_elemento.at(2));
    if (game.gioco == 2 && giocatori.at(0).punteggio != 10) {
        std::string frase = stringhe.frasi.at(25).at(3) + stringhe.frasi.at(3).at(4);
        DrawText(frase.c_str(), (screenWidth - MeasureText(frase.c_str(), dimensione_1))/2, screenHeight/2 - 35, dimensione_1, colore);
        DrawText(stringhe.frasi.at(3).at(1).c_str(), (screenWidth - MeasureText(stringhe.frasi.at(3).at(1).c_str(), dimensione_2))/2, screenHeight/2 + 35, dimensione_2, colore);
    } else  if (game.gioco == 3 && giocatori.at(1).punteggio != 10) {
        std::string frase = stringhe.frasi.at(25).at(4) + stringhe.frasi.at(3).at(4);
        DrawText(frase.c_str(), (screenWidth - MeasureText(frase.c_str(), dimensione_1))/2, screenHeight/2 - 35, dimensione_1, colore);
        DrawText(stringhe.frasi.at(3).at(1).c_str(), (screenWidth - MeasureText(stringhe.frasi.at(3).at(1).c_str(), dimensione_2))/2, screenHeight/2 + 35, dimensione_2, colore);
    } else if (game.gioco == 0) {
        DrawText(stringhe.frasi.at(3).at(0).c_str(), (screenWidth - MeasureText(stringhe.frasi.at(3).at(0).c_str(), dimensione_1))/2, screenHeight/2, dimensione_1, colore);
    } else if ( giocatori.at(0).punteggio == 10 || giocatori.at(1).punteggio == 10 || game.pausa) {
        if (giocatori.at(0).punteggio == 10) {
            std::string frase = stringhe.frasi.at(25).at(3) + stringhe.frasi.at(3).at(5);
            DrawText(frase.c_str(), (screenWidth - MeasureText(frase.c_str(), dimensione_1))/2, screenHeight/2 - 35, dimensione_1, colore);
        } else if (giocatori.at(1).punteggio == 10) {
            std::string frase = stringhe.frasi.at(25).at(4) + stringhe.frasi.at(3).at(5);
            DrawText(frase.c_str(), (screenWidth - MeasureText(frase.c_str(), dimensione_1)) / 2, screenHeight / 2 - 35, dimensione_1,
                     colore);
        } else {
            DrawText(stringhe.frasi.at(3).at(7).c_str(), (screenWidth - MeasureText(stringhe.frasi.at(3).at(7).c_str(), dimensione_1)) / 2, screenHeight /2 - 35, dimensione_1,colore);
        }
        DrawText(stringhe.frasi.at(3).at(2).c_str(), (screenWidth - MeasureText(stringhe.frasi.at(3).at(2).c_str(), dimensione_2))/2, screenHeight/2 + 45, dimensione_2, colore);
        DrawText(stringhe.frasi.at(3).at(3).c_str(), (screenWidth - MeasureText(stringhe.frasi.at(3).at(3).c_str(), dimensione_2))/2, screenHeight/2 + 105, dimensione_2, colore);

    }
}

void inc_v (Speeds &velocita) {
    if (velocita.v_ball_x > 0) {
        velocita.v_ball_x += 0.005;
    } else {
        velocita.v_ball_x -= 0.005;
    }
    if (velocita.v_ball_y > 0) {
        velocita.v_ball_y += 0.005;
    } else {
        velocita.v_ball_y -= 0.005;
    }
}

void repet (Game &game, std::vector<Player> &giocatori) {
    game.gioco = 4;
    game.tempo_partita = 0;
    giocatori.at(0).barretta.height = game.h_iniz;
    giocatori.at(1).barretta.height = game.h_iniz;
}

void dis_caricamento (Rectangle &caricamento, Game &game, const String &stringhe, const Colors &colori) {
    int screenWidth = game.screenWidth, screenHeight = game.screenHeight;
    Color colore = colori.colore_elementi.at(colori.scelta_elemento.at(2));
    caricamento.width += GetRandomValue(0, 75);

    if (caricamento.width >= screenWidth) {
        game.stato = game.stato_successivo;
        caricamento.width= 0;
    }
    DrawRectangleRec(caricamento,  colori.colore_elementi.at(colori.scelta_elemento.at(1)));
    if (game.tempo < 20) {
        DrawText(stringhe.frasi.at(1).at(0).c_str(), screenWidth * 0.05, screenHeight - 2 * caricamento.height, stringhe.dim_stringhe.at(3), colore);
    } else if (game.tempo < 40) {
        DrawText(stringhe.frasi.at(1).at(1).c_str(), screenWidth * 0.05, screenHeight - 2 * caricamento.height, stringhe.dim_stringhe.at(3), colore);
    } else if (game.tempo < 60) {
        DrawText(stringhe.frasi.at(1).at(2).c_str(), screenWidth * 0.05, screenHeight - 2 * caricamento.height, stringhe.dim_stringhe.at(3), colore);
    }
}

void dis_home_stato_0 (const Game &game, const Colors &colori, const String &stringhe){
    int screenWidth = game.screenWidth, screenHeight = game.screenHeight;
    Color colore = colori.colore_elementi.at(colori.scelta_elemento.at(2));
    DrawText(stringhe.frasi.at(0).at(0).c_str(), screenWidth/2 - (float)(MeasureText(stringhe.frasi.at(0).at(0).c_str(), stringhe.dim_stringhe.at(0)))/2, screenHeight * 0.02, stringhe.dim_stringhe.at(0), colore);
    DrawText(stringhe.frasi.at(0).at(1).c_str(), screenWidth * 0.02, screenHeight * 0.7, stringhe.dim_stringhe.at(1), colore);
    DrawText(stringhe.frasi.at(0).at(2).c_str(), screenWidth * 0.02, screenHeight * 0.8, stringhe.dim_stringhe.at(1), colore);
}

void home_stato_0 (Game &game, const String &stringhe){
    int screenHeight = game.screenHeight;
    float dimnsione =  stringhe.dim_stringhe.at(1);
    float x = GetScreenWidth() * 0.02, y1 = screenHeight * 0.7, y2 = screenHeight * 0.8;
    if (CheckCollisionPointRec(GetMousePosition(), {x, y1, (float)MeasureText(stringhe.frasi.at(0).at(1).c_str(), dimnsione), dimnsione}) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        game.stato = 1;
        game.stato_successivo = 2;
    } else if (CheckCollisionPointRec(GetMousePosition(), {x , y2, (float)MeasureText(stringhe.frasi.at(0).at(2).c_str(), dimnsione), dimnsione}) &&
               IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        game.stato = 1;
        game.stato_successivo = 20;
    }
    game.gioco = 0;
}

void partita_classica_stato_3 (Game &game, std::vector<Player> &giocatori, Ball &pallina, Speeds &velocita, const String &stringhe) {

    float screenWidth = game.screenWidth, screenHeight = game.screenHeight;

    if (!game.pausa) {

        if (game.gioco == 0) {
            giocatori.at(0).punteggio = 0;
            giocatori.at(1).punteggio = 0;
            game.tempo_partita = 0;
            giocatori.at(0).barretta = {valore_scalato(game, 50, true), screenHeight / 2 - game.h_iniz / 2, giocatori.at(0).barretta.width, game.h_iniz};
            giocatori.at(1).barretta = {(float)(screenWidth - valore_scalato(game, 50, true)), screenHeight / 2 - game.h_iniz / 2, giocatori.at(0).barretta.width, game.h_iniz};
            if (pallina.x != screenWidth / 2) {
                pallina.x = screenWidth / 2;
                pallina.y = (float)(GetRandomValue(pallina.raggio, screenHeight - pallina.raggio));
            }

            if (IsKeyPressed(KEY_SPACE)) {
                game.gioco = 4;
                vi_X(velocita);
                vi_y(velocita);
                if (velocita.v_ball_x < 0) {
                    pallina.rimbalzo = true;
                } else {
                    pallina.rimbalzo = false;
                }
            }
        } else if (game.gioco > 1) {

            agg_pos_pallina_y(pallina, velocita);
            agg_pos_pallina_x(game, pallina, giocatori, velocita );
            agg_pos_barretta_y_1(velocita, giocatori);
            agg_pos_barretta_y_2(velocita, giocatori);

            if (pallina.x <= 0 || pallina.x >= screenWidth) {
                if (pallina.x <= 0) {
                    giocatori.at(1).punteggio++;
                    game.gioco = 3;
                } else if (pallina.x >= screenWidth) {
                    giocatori.at(0).punteggio++;
                    game.gioco = 2;
                }
                pallina.x = GetRandomValue(pallina.raggio, screenHeight - pallina.raggio);
                pallina.x = screenWidth / 2;
                velocita.v_ball_x = 0;
                velocita.v_ball_y = 0;
            }

            if (IsKeyPressed(KEY_SPACE) && giocatori.at(0).punteggio < 10 && giocatori.at(1).punteggio < 10){
                if (game.gioco == 2) {
                    velocita.v_ball_x = -velocita.v0;
                    pallina.rimbalzo = true;
                } else if (game.gioco == 3){
                    velocita.v_ball_x = velocita.v0;
                    pallina.rimbalzo = false;
                }
                vi_y(velocita);
                repet(game, giocatori);
            }

            game.tempo_partita++;
            if (game.tempo_partita > 1500 && game.gioco != 2 && game.gioco != 3) {
                inc_v(velocita);
                game.incremento += valore_scalato(game, 0.005, true);
                if (game.tempo_partita > 2000 && giocatori.at(0).barretta.height > valore_scalato(game, 30, false)) {
                    giocatori.at(0).barretta.height -= valore_scalato(game, 0.05, true);
                    giocatori.at(1).barretta.height -= valore_scalato(game, 0.05, true);

                }
            }


        }
    }

    if (giocatori.at(0).punteggio == 10 || giocatori.at(1).punteggio == 10 || game.pausa) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {(screenWidth - MeasureText(stringhe.frasi.at(3).at(2).c_str(), stringhe.dim_stringhe.at(3))) / 2, screenHeight / 2 + 45, (float)MeasureText(stringhe.frasi.at(3).at(2).c_str(), stringhe.dim_stringhe.at(3)), stringhe.dim_stringhe.at(3)})) {
            game.gioco = 0;
            game.pausa = false;
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {(screenWidth - MeasureText(stringhe.frasi.at(3).at(3).c_str(), stringhe.dim_stringhe.at(3))) / 2, screenHeight / 2 + 105, (float)MeasureText(stringhe.frasi.at(3).at(2).c_str(), stringhe.dim_stringhe.at(3)), stringhe.dim_stringhe.at(3)})) {
            game.stato = 1;
            game.stato_successivo = 0;
            game.pausa = false;
        }
    }
}

void dis_partita_classica_stato_3 (Game &game, const Ball &pallina,  std::vector<Player> &giocatori, const Colors &colori, const String &stringhe) {
    int screenWidth = game.screenWidth, ScreenHeight = game.screenHeight;
    interruzioni(game, giocatori, colori, stringhe);
    DrawCircle(pallina.x, pallina.y, pallina.raggio, colori.colore_elementi.at(colori.scelta_elemento.at(0)));
    DrawRectangleRec(giocatori.at(0).barretta, colori.colore_elementi.at(colori.scelta_elemento.at(1)));
    DrawRectangleRec(giocatori.at(1).barretta, colori.colore_elementi.at(colori.scelta_elemento.at(1)));
    dis_punteggio(giocatori.at(0).punteggio, screenWidth / 4, colori);
    dis_punteggio(giocatori.at(1).punteggio, screenWidth * 0.75, colori);
    if (!(game.gioco == 0 || game.gioco == 2 || game.gioco == 3 || game.pausa)) {
        for (int i = 20; i <= ScreenHeight; i += 100) {
            DrawRectangle((screenWidth / 2) - 3, i, 6, 40, colori.colore_elementi.at(colori.scelta_elemento.at(1)));
        }
    }


}

void dis_modalita_stato_2(const Game &game, const Colors &colori, const String &stringhe) {
    int screenWidth = game.screenWidth, screenHeight = game.screenHeight / 2 - 280;
    Color colore = colori.colore_elementi.at(colori.scelta_elemento.at(2));
    DrawText(stringhe.frasi.at(2).at(0).c_str(), (screenWidth - MeasureText(stringhe.frasi.at(2).at(0).c_str(), stringhe.dim_stringhe.at(1))) / 2, screenHeight * 0.02, stringhe.dim_stringhe.at(1), colore);
    stampa_elenco(game, stringhe, colori, 2, 1, 8);

}


void dis_partita_libera_stato_4 (Game &game, const Ball &pallina,  std::vector<Player> &giocatori, const Colors &colori, const String &stringhe) {
    dis_partita_classica_stato_3(game, pallina, giocatori, colori, stringhe);
    int screenWidth =game.screenWidth, screenHeight = game.screenHeight;
    if (!(game.gioco == 0 || game.gioco == 2 || game.gioco == 3 || game.pausa)) {
        for (int i = 0; i < screenHeight; i += 80) {
            DrawRectangle((screenWidth / 3) - 3, i, 6, 20, colori.colore_elementi.at(colori.scelta_elemento.at(1)));
        }
        for (int i = 0; i < screenHeight; i += 80) {
            DrawRectangle((screenWidth * 2 / 3) - 3, i, 6, 20, colori.colore_elementi.at(colori.scelta_elemento.at(1)));
        }
    }
}

void scelta_difficolta(Ball &pallina, Speeds &velocita, Game &game, int difficolta) {
    if (difficolta == 0) {
        velocita.v0 = valore_scalato(game, 9, true);
        velocita.v_barretta_1 = valore_scalato(game, 7, true);
        velocita.v_barretta_2 = valore_scalato(game, 7, true);
        game.h_iniz = valore_scalato(game, 200, false);
        pallina.raggio = valore_scalato(game, 15, false);
    } else if (difficolta == 1) {
        velocita.v0 = valore_scalato(game, 11, true);
        velocita.v_barretta_1 = valore_scalato(game, 9, true);
        velocita.v_barretta_2 = valore_scalato(game, 9, true);
        game.h_iniz = valore_scalato(game, 180, false);
        pallina.raggio = valore_scalato(game, 13, false);
    } else {
        velocita.v0 = valore_scalato(game, 15, true);
        velocita.v_barretta_1 = valore_scalato(game, 11, true);
        velocita.v_barretta_2 = valore_scalato(game, 11, true);
        game.h_iniz = valore_scalato(game, 160, false);
        pallina.raggio = valore_scalato(game, 10, false);
    }
}

void modalita_stato_2 (Ball &pallina, Speeds &velocita, Game &game , const String &stringhe) {
    int valore = interazione_elenco(game, stringhe, 2, 1, 8);
    if (valore > 0) {
        if (valore< 3) {
            scelta_difficolta(pallina, velocita, game, 0);
        } else if (valore< 5) {
            scelta_difficolta(pallina, velocita, game, 1);
        } else if (valore< 7) {
            scelta_difficolta(pallina, velocita, game, 2);
        }
        if (valore% 2 != 0 && valore!= 7) {
            game.stato_successivo = 3;
            game.stato= 1;
        } else if (valore!= 7) {
            game.stato_successivo = 4;
            game.stato= 1;
        } else {
            game.stato = 19;
        }
    }
}

void partita_libera_stato_4 (Game &game,std::vector<Player> &giocatori,Ball &pallina,Speeds &velocita, const String &stringhe) {
    partita_classica_stato_3(game, giocatori, pallina, velocita, stringhe);
    if (game.gioco > 1 && !game.pausa) {
        agg_pos_barretta_x_1(game, velocita, giocatori);
        agg_pos_barretta_x_2(game, velocita, giocatori);
    }
}



void scelta_elemento_colorare_stato_21(const String &stringhe, Game &game, Colors &colori){
    int valore = interazione_elenco(game, stringhe, 21, 1, 5);

    if (valore != 0) {
        game.stato = 22;
        if (valore == 1) {
            colori.elemento_da_colorare = 0;
        } else if (valore == 2) {
            colori.elemento_da_colorare = 1;
        } else if (valore == 3) {
            colori.elemento_da_colorare = 2;
        } else if (valore == 4) {
            colori.elemento_da_colorare = 3;
        }
    }
    if (IsKeyDown(KEY_R) && IsKeyDown(KEY_G) && IsKeyDown(KEY_B) && !IsKeyDown(KEY_N)&&!colori.RGB_Yes_No) {
        colori.scelta_elemento = {4, 4, 4, 5};
        colori.RGB_Yes_No = true;
    } else if (IsKeyDown(KEY_R) && IsKeyDown(KEY_G) && IsKeyDown(KEY_B) && IsKeyDown(KEY_N)&&colori.RGB_Yes_No) {
        colori.scelta_elemento = {0, 1, 2, 3};
        colori.RGB_Yes_No = false;
    }
}



void rgb (Colors &colori) {
    int velocita = 3;
    if (colori.rgb_1.cresci) {
        if (colori.rgb_1.canali_colore.at(0) < 250) { colori.rgb_1.canali_colore.at(0) += velocita; }
        else if (colori.rgb_1.canali_colore.at(1) < 250) { colori.rgb_1.canali_colore.at(1) += velocita; }
        else if (colori.rgb_1.canali_colore.at(2) < 250) { colori.rgb_1.canali_colore.at(2) += velocita; } else {colori.rgb_1.cresci = false;}
    }
    else {
        if (colori.rgb_1.canali_colore.at(0) > 10) { colori.rgb_1.canali_colore.at(0) -= velocita; }
        else if (colori.rgb_1.canali_colore.at(1) > 10) { colori.rgb_1.canali_colore.at(1) -= velocita; }
        else if (colori.rgb_1.canali_colore.at(2) > 10) { colori.rgb_1.canali_colore.at(2) -= velocita; } else { colori.rgb_1.cresci =true;}
    }
    colori.rgb_1.colore = {colori.rgb_1.canali_colore.at(0), colori.rgb_1.canali_colore.at(1), colori.rgb_1.canali_colore.at(2), 255};
    colori.colore_elementi.at(4) = colori.rgb_1.colore;

    if (colori.rgb_2.cresci) {
        if (colori.rgb_2.canali_colore.at(0) < 250) { colori.rgb_2.canali_colore.at(0) += velocita; }
        else if (colori.rgb_2.canali_colore.at(1) < 250) { colori.rgb_2.canali_colore.at(1) += velocita; }
        else if (colori.rgb_2.canali_colore.at(2) < 250) { colori.rgb_2.canali_colore.at(2) += velocita; } else {colori.rgb_2.cresci = false;}
    }
    else {
        if (colori.rgb_2.canali_colore.at(0) > 10) { colori.rgb_2.canali_colore.at(0) -= velocita; }
        else if (colori.rgb_2.canali_colore.at(1) > 10) { colori.rgb_2.canali_colore.at(1) -= velocita; }
        else if (colori.rgb_2.canali_colore.at(2) > 10) { colori.rgb_2.canali_colore.at(2) -= velocita; } else { colori.rgb_2.cresci =true;}
    }
    colori.rgb_2.colore = {colori.rgb_2.canali_colore.at(0), colori.rgb_2.canali_colore.at(1), colori.rgb_2.canali_colore.at(2), 255};
    colori.colore_elementi.at(5) = colori.rgb_2.colore;
}


void full_screen (const int screenWidth) {
    if (IsKeyPressed(KEY_F11))
    {
        // see what display we are on right now
        int display = GetCurrentMonitor();


        if (IsWindowFullscreen())
        {
            // if we are full screen, then go back to the windowed size
            SetWindowSize(screenWidth, GetScreenHeight());
        }
        else
        {
            // if we are not full screen, set the window size to match the monitor we are on
            SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        }

        // toggle the state
        ToggleFullscreen();
    }
}

void colori_custom_stato_23 (const Game &game, float pos_partenza, Colors &colori) {
    float x = game.screenWidth/5;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {x , pos_partenza, 40, 255})){
        colori.colori_custom.at(colori.elemento_da_colorare).r = GetMouseY() - pos_partenza;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {x + 80 , pos_partenza, 40, 255})){
        colori.colori_custom.at(colori.elemento_da_colorare).g = GetMouseY() - pos_partenza;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {x + 160 , pos_partenza, 40, 255})){
        colori.colori_custom.at(colori.elemento_da_colorare).b = GetMouseY() - pos_partenza;
    }
}

void dis_colori_custom_stato_23 (const Colors &colori, const String &stringhe, const Game &game) {
    int screenWidth = game.screenWidth, screenHeight = game.screenHeight;
    float x = screenWidth/5, pos_partenza = screenHeight/2 - 256;
    DrawRectangle(x - 10, pos_partenza-10, 220, 286, GRAY);
    DrawRectangle(x, pos_partenza, 40, 256 + 10, RED);
    DrawRectangle(x + 5, colori.colori_custom.at(colori.elemento_da_colorare).r + pos_partenza, 30, 10, BLACK);
    DrawRectangle(x + 80, pos_partenza, 40, 256 + 10, GREEN);
    DrawRectangle(x + 85, colori.colori_custom.at(colori.elemento_da_colorare).g + pos_partenza, 30, 10, BLACK);
    DrawRectangle(x + 160, pos_partenza, 40, 256 + 10, BLUE);
    DrawRectangle(x + 165, colori.colori_custom.at(colori.elemento_da_colorare).b + pos_partenza, 30, 10, BLACK);

    DrawRectangle(x - 10, pos_partenza + 306, 220, 50, GRAY);
    std::string rosso;
    rosso = std::to_string(colori.colori_custom.at(colori.elemento_da_colorare).r);
    DrawText(rosso.c_str(), x - 5, pos_partenza + 316, 30, RED);
    std::string verde;
    verde = std::to_string(colori.colori_custom.at(colori.elemento_da_colorare).g);
    DrawText(verde.c_str(), x + 75, pos_partenza + 316, 30, GREEN);
    std::string blu;
    blu = std::to_string(colori.colori_custom.at(colori.elemento_da_colorare).b);
    DrawText(blu.c_str(), x + 155, pos_partenza + 316, 30, BLUE);

    DrawRectangle(screenWidth * 4 / 5 - 266, pos_partenza - 10, 276, 276, GRAY);
    DrawRectangle(screenWidth * 4 / 5 - 256, pos_partenza, 256, 256, colori.colori_custom.at(colori.elemento_da_colorare));
    DrawText(stringhe.frasi.at(23).at(0).c_str(), (screenWidth - MeasureText(stringhe.frasi.at(23).at(0).c_str(), stringhe.dim_stringhe.at(2))) / 2, screenHeight * 0.80, stringhe.dim_stringhe.at(2), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
}


void dis_impostazioni_stato_20 (const Game &game, const String &stringhe, const Colors &colori) {
    float x = game.screenWidth, y = game.screenHeight;
    Color colore = colori.colore_elementi.at(colori.scelta_elemento.at(2));
    DrawText(stringhe.frasi.at(20).at(0).c_str(), (x - MeasureText(stringhe.frasi.at(20).at(0).c_str(), stringhe.dim_stringhe.at(1)))/2, y*0.02, stringhe.dim_stringhe.at(1),colore);
    stampa_elenco(game, stringhe, colori, 20, 1, 5);

}

void impostazioni_stato_20 (Game &game, const String &stringhe) {
    int valore;

    valore = interazione_elenco(game, stringhe, 20, 1, 5);
    if (valore != 0) {
        if (valore == 1) {
            game.stato_successivo = 25;
        } else if (valore == 2) {
            game.stato_successivo = 21;
        } else if (valore == 3) {
            game.stato_successivo = 27;
        } else if (valore == 4){
            game.stato_successivo = 26;
        }
        game.stato = 1;
    }


}

void dis_nomi_stato_25 (const Colors &colori, const String &stringhe, const Game &game) {
    float x = game.screenWidth, y = game.screenHeight;
    DrawText(stringhe.frasi.at(25).at(0).c_str(),  x/2 - MeasureText(stringhe.frasi.at(25).at(0).c_str(), stringhe.dim_stringhe.at(1))/2, y * 0.02, stringhe.dim_stringhe.at(1), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(25).at(1).c_str(), x/2 - MeasureText(stringhe.frasi.at(25).at(1).c_str(), stringhe.dim_stringhe.at(2))/2, y *0.30, stringhe.dim_stringhe.at(2), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(25).at(2).c_str(),  x/2 - MeasureText(stringhe.frasi.at(25).at(2).c_str(), stringhe.dim_stringhe.at(2))/2, y * 0.60, stringhe.dim_stringhe.at(2), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(25).at(3).c_str(), x/2 - MeasureText(stringhe.frasi.at(25).at(3).c_str(), stringhe.dim_stringhe.at(3))/2, y *0.40 , stringhe.dim_stringhe.at(3), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(25).at(4).c_str(), x/2 - MeasureText(stringhe.frasi.at(25).at(4).c_str(), stringhe.dim_stringhe.at(3))/2, y * 0.70, stringhe.dim_stringhe.at(3), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    if (game.inserimento_nome == 1) {
        if (game.tempo > 30) {
            DrawText("_", x/2 + MeasureText(stringhe.frasi.at(25).at(3).c_str(), stringhe.dim_stringhe.at(3))/2 + 10, y *0.40, stringhe.dim_stringhe.at(3), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
        }
    } else if (game.inserimento_nome == 2) {
        if (game.tempo > 30) {
            DrawText("_", x/2 + MeasureText(stringhe.frasi.at(25).at(4).c_str(), stringhe.dim_stringhe.at(3))/2 + 10, y *0.70, stringhe.dim_stringhe.at(3), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
        }
    }
}

void modifica_nome(String &stringhe, int limite, int n_stato, int n_frase){
    char c = GetCharPressed();
    if (c > 0 && stringhe.frasi.at(n_stato).at(n_frase).size() < limite) {
        stringhe.frasi.at(n_stato).at(n_frase) += c;
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (!stringhe.frasi.at(n_stato).at(n_frase).empty())
            stringhe.frasi.at(n_stato).at(n_frase).pop_back();
    }
}

void nomi_stato_25 (String &stringhe, Game &game) {
    int limite = 20;
    float x = game.screenWidth, y = game.screenHeight * 0.30 + stringhe.dim_stringhe.at(1), dimensione = stringhe.dim_stringhe.at(2);
    float incremento = GetScreenHeight() * 0.30;
    if (CheckCollisionPointRec(GetMousePosition(), {x/2 - MeasureText(stringhe.frasi.at(25).at(3).c_str(), dimensione)/2, y, (float)MeasureText(stringhe.frasi.at(1).at(0).c_str(), dimensione), dimensione}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        game.inserimento_nome = 1;
    } else if (CheckCollisionPointRec(GetMousePosition(), {x/2 - MeasureText(stringhe.frasi.at(25).at(4).c_str(), dimensione)/2, y + incremento, (float)MeasureText(stringhe.frasi.at(1).at(1).c_str(), dimensione), dimensione}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        game.inserimento_nome = 2;
    } else if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        game.inserimento_nome = 0;
    }
    if (game.inserimento_nome == 1) {
        modifica_nome(stringhe, limite, 25, 3);
    } else if (game.inserimento_nome == 2) {
        modifica_nome(stringhe, limite, 25, 4);
    }

}

void dis_crediti_stato_26 (const Game &game, const String &stringhe, const Colors &colori) {
    float x = game.screenWidth, y =game.screenHeight;
    DrawText(stringhe.frasi.at(26).at(0).c_str(),  x/2 - MeasureText(stringhe.frasi.at(26).at(0).c_str(), stringhe.dim_stringhe.at(1))/2, y*0.02, stringhe.dim_stringhe.at(1), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(26).at(1).c_str(),  x/2 - MeasureText(stringhe.frasi.at(26).at(1).c_str(), stringhe.dim_stringhe.at(2))/2, y/4, stringhe.dim_stringhe.at(2), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(26).at(2).c_str(),  x/2 - MeasureText(stringhe.frasi.at(26).at(2).c_str(), stringhe.dim_stringhe.at(2))/2, y/2, stringhe.dim_stringhe.at(2), colori.colore_elementi.at(colori.scelta_elemento.at(1)));

}

void dis_sposta_tra_stati(const Triangle &triangolo, const Colors &colori) {
    DrawTriangle(triangolo.punto_1, triangolo.punto_2, triangolo.punto_3, colori.colore_elementi.at(colori.scelta_elemento.at(1)));
}



void dis_tasti_stato_27 (const Game &game, const String &stringhe, const Colors &colori) {
    float x = game.screenWidth, y = game.screenHeight;
    Color colore = colori.colore_elementi.at(colori.scelta_elemento.at(2));
    int incremento_pag = game.pagina_tasti * 5;
    if (game.pagina_tasti == 0) {
        DrawText(stringhe.frasi.at(27).at(0).c_str(), (x - MeasureText(stringhe.frasi.at(27).at(0).c_str(), stringhe.dim_stringhe.at(1)))/2, y*0.02, stringhe.dim_stringhe.at(1),colore);
    } else if (game.pagina_tasti == 1) {
        DrawText(stringhe.frasi.at(25).at(3).c_str(), (x - MeasureText(stringhe.frasi.at(25).at(3).c_str(), stringhe.dim_stringhe.at(1)))/2, y*0.02, stringhe.dim_stringhe.at(1),colore);
    } else if (game.pagina_tasti == 2) {
        DrawText(stringhe.frasi.at(25).at(4).c_str(), (x - MeasureText(stringhe.frasi.at(25).at(4).c_str(), stringhe.dim_stringhe.at(1)))/2, y*0.02, stringhe.dim_stringhe.at(1),colore);
    }
    stampa_elenco(game, stringhe, colori, 27, 1 + incremento_pag, 4 + incremento_pag + 1);
}

int stato_27_cambia_pagina (const Game &game, const Triangle &triangolo_indietro, const Triangle &triangolo_avanti) {
    if (game.pagina_tasti > 0) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointTriangle(GetMousePosition(), triangolo_indietro.punto_1, triangolo_indietro.punto_2, triangolo_indietro.punto_3)) {
            return -1;
        }
    }
    if (game.pagina_tasti < 2) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointTriangle(GetMousePosition(), triangolo_avanti.punto_1, triangolo_avanti.punto_2, triangolo_avanti.punto_3)) {
            return 1;
        }
    }
    return 0;
}

void dis_info_modalita_stato_19 (const Game &game, const String &stringhe, const Colors &colori) {
    float x = game.screenWidth, y = game.screenHeight;
    float dimensione_3 = stringhe.dim_stringhe.at(3);
    DrawText(stringhe.frasi.at(19).at(0).c_str(),  x/2 - MeasureText(stringhe.frasi.at(19).at(0).c_str(), stringhe.dim_stringhe.at(1))/2, y * 0.02, stringhe.dim_stringhe.at(1), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(19).at(1).c_str(), x/2 - MeasureText(stringhe.frasi.at(19).at(1).c_str(), dimensione_3)/2, y *0.15 + dimensione_3, dimensione_3, colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(19).at(2).c_str(),  x/2 - MeasureText(stringhe.frasi.at(19).at(2).c_str(), dimensione_3)/2, y *0.15 + dimensione_3*2, dimensione_3, colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(19).at(3).c_str(), x/2 - MeasureText(stringhe.frasi.at(19).at(3).c_str(), dimensione_3)/2, y *0.32 , dimensione_3, colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(19).at(4).c_str(), x/2 - MeasureText(stringhe.frasi.at(19).at(4).c_str(), dimensione_3)/2, y *0.32 + dimensione_3, dimensione_3, colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(19).at(5).c_str(), x/2 - MeasureText(stringhe.frasi.at(19).at(5).c_str(), stringhe.dim_stringhe.at(1))/2, y/2 , stringhe.dim_stringhe.at(1), colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(19).at(6).c_str(), x/2 - MeasureText(stringhe.frasi.at(19).at(6).c_str(), dimensione_3)/2, y * 0.65, dimensione_3, colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(19).at(7).c_str(), x/2 - MeasureText(stringhe.frasi.at(19).at(7).c_str(), dimensione_3)/2, y * 0.65 + dimensione_3, dimensione_3, colori.colore_elementi.at(colori.scelta_elemento.at(2)));
    DrawText(stringhe.frasi.at(19).at(8).c_str(), x/2 - MeasureText(stringhe.frasi.at(19).at(8).c_str(), dimensione_3)/2, y * 0.65 + dimensione_3*2, dimensione_3, colori.colore_elementi.at(colori.scelta_elemento.at(2)));
}


void disegna_cursore ( const Cursor &cursore){
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        DrawTexture(cursore.cliccato, GetMouseX() - 7.5, GetMouseY(), WHITE);
    } else {
        DrawTexture(cursore.non_cliccato, GetMouseX() - 7.5, GetMouseY(), WHITE);
    }
}

std::vector<std::string> split(const std::string& s, char delimiter = ' ') {
    std::stringstream in(s);
    std::string temp;
    std::vector<std::string> v;
    while(getline(in, temp, delimiter)){
        v.push_back(temp);
    }
    return v;
}

void carica_nomi (String &stringhe) {
    std::vector<std::string> temp;
    std::string riga;
    std::ifstream in ("file/nomi.csv");
    if (in) {
        std::getline(in, riga);
        temp = split(riga, ';');
        stringhe.frasi.at(25).at(3) = temp.at(0);
        stringhe.frasi.at(25).at(4) = temp.at(1);
    } else {
        std::cout << "Errore nel caricamento dei nomi, sono stati caricati i nomi di base" << std::endl;
    }
    in.close();
}

void salva_nomi (const String &stringhe) {
    std::ofstream nomi ("file/nomi.csv");
    if (nomi) {
        nomi << stringhe.frasi.at(25).at(3) << ';' << stringhe.frasi.at(25).at(4) << ';';
    } else {
        std::cout << "Errore nel salvataggio dei nomi" << std::endl;
    }

    nomi.close();
}

void carica_colori (Colors &colori){
    std::vector<std::string> temp;
    std::string riga;
    std::ifstream in ("file/colori.csv");

    if (in) {
        for (int j = 0; j < 2; ++j) {
            std::getline(in, riga);
            temp = split(riga, ';');
            for (int i = 0; i < 4; ++i) {
                if (j == 0) {
                    colori.colore_elementi.at(i).r = (unsigned char)(std::stoi(temp.at(i*3)));
                    colori.colore_elementi.at(i).g = (unsigned char)(std::stoi(temp.at(i*3+1)));
                    colori.colore_elementi.at(i).b = (unsigned char)(std::stoi(temp.at(i*3+2)));
                } else {
                    colori.colori_custom.at(i).r = (unsigned char)(std::stoi(temp.at(i*3)));
                    colori.colori_custom.at(i).g = (unsigned char)(std::stoi(temp.at(i*3+1)));
                    colori.colori_custom.at(i).b = (unsigned char)(std::stoi(temp.at(i*3+2)));
                }
            }
        }
    } else {
        std::cout << "Errore nel caricamento dei colori, quindi sono stati caricati i colori base" << std::endl;
    }
    in.close();
}

void salva_colori (const Colors &colori) {
    std::ofstream out ("file/colori.csv");
    if (out) {
        for (int i = 0; i < 4; ++i) {
            out << (int)(colori.colore_elementi.at(i).r) << ';' << (int)(colori.colore_elementi.at(i).g) << ';' <<  (int)(colori.colore_elementi.at(i).b) << ';';
        }
        out << std::endl;
        for (int i = 0; i < 4; ++i) {
            out << (int)(colori.colori_custom.at(i).r) << ';' << (int)(colori.colori_custom.at(i).g) << ';' <<  (int)(colori.colori_custom.at(i).b) << ';';
        }
    } else {
        std::cout << "Errore nel salvataggio dei colori" << std::endl;
    }

    out.close();
}


void riproduci_audio (Sound fxWav, Sound fxOgg) {
    if (IsKeyPressed(KEY_SPACE)) PlaySound(fxWav);      // Play WAV sound
    if (IsKeyPressed(KEY_ENTER)) PlaySound(fxOgg);      // Play OGG sound
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [audio] example - sound loading and playing");

    InitAudioDevice();      // Initialize audio device

    Sound fxWav = LoadSound("assets/chill.mp3");         // Load WAV audio file
    Sound fxOgg = LoadSound("assets/partita.mp3");        // Load OGG audio file

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        riproduci_audio(fxWav, fxOgg);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Press SPACE to PLAY the WAV sound!", 200, 180, 20, LIGHTGRAY);
        DrawText("Press ENTER to PLAY the OGG sound!", 200, 220, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadSound(fxWav);     // Unload sound data
    UnloadSound(fxOgg);     // Unload sound data

    CloseAudioDevice();     // Close audio device

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}