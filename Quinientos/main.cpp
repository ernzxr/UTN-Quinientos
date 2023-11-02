#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <Windows.h>
#include <mmsystem.h>

using namespace std;

#include "rlutil.h"
#include "color.h"
#pragma comment(lib, "winmm.lib") /// VER .h
#include "musica.h"
#include "dados.h"
#include "interfaz.h"
#include "menu_principal.h"
#include "jugar_partida.h"


int main(){
    int const CANTIDAD_JUGADORES=4, TOP=10;
    int opcion, opcionRondas, maxRondas, ronda, tiradasTotales, jugador, jugadores, validarGanador, cQuinientos, cEscalera;
    int mPuntajeRondaJugador[CANTIDAD_JUGADORES][20], vTiradasTotales[CANTIDAD_JUGADORES], vAcuPuntajeJugador[CANTIDAD_JUGADORES], vDados[6], mPuntajesTiradas[CANTIDAD_JUGADORES][3];
    char mJugadores[CANTIDAD_JUGADORES][8], mRanking[TOP][8];
    bool escalera, quinientos, mute=false, iniciarPartida, volverInicio;
    bool vGanadorEscalera[CANTIDAD_JUGADORES], vGanadorQuinientos[CANTIDAD_JUGADORES];

    inicializarSemilla();

    colorFondo();

    if(!mute)musicaMenuPrincipal(); /// MUSICA

    while(true){
        rlutil::cls();

        rlutil::hidecursor();

        opcion = menuPrincipal(); /// MENU PRINCIPAL

        switch(opcion){

            case 1:
                /// UN JUGADOR
                iniciarPartida=true;
                opcionRondas = menuMaxRondas();
                switch(opcionRondas){
                    case 1:
                        maxRondas=1;
                        break;
                    case 2:
                        maxRondas=5;
                        break;
                    case 3:
                        maxRondas=10;
                        break;
                    case 0:
                        iniciarPartida=false;
                        break;
                }
                while(iniciarPartida){
                    /// INICIALIZACION DE JUEGO
                    ronda=0, tiradasTotales=0, jugador=0;
                    escalera=false, quinientos=false;
                    reiniciarPartida(mPuntajeRondaJugador, mPuntajesTiradas, vAcuPuntajeJugador, vTiradasTotales, vGanadorEscalera, vGanadorQuinientos, CANTIDAD_JUGADORES);

                    cargarNombre(mJugadores, 7, jugador);

                    if(!mute)playMusicaPartida(); /// MUSICA PARTIDA

                    /// SE JUEGAN LA CANTIDAD MAXIMA DE RONDAS INDICADAS
                    while(ronda<maxRondas && !escalera && !quinientos){
                        ronda++;
                        jugarRonda(vDados, mPuntajesTiradas, mPuntajeRondaJugador, vGanadorEscalera, vTiradasTotales, mJugadores, ronda, jugadores);
                        if(!vGanadorEscalera[jugador]){
                            /// GUARDAR MAXIMO PUNTAJE DE LAS TRES TIRADAS EN LA RONDA CORRESPONDIENTE
                            cargarPuntajes(mPuntajeRondaJugador, mPuntajesTiradas, vAcuPuntajeJugador, ronda-1, jugador);
                            mostrarPuntajeRonda(mPuntajeRondaJugador[jugador][ronda-1], jugador);
                            if(vAcuPuntajeJugador[jugador]>=500){
                                /// FINALIZA JUEGO POR SUMAR QUINIENTOS PUNTOS
                                quinientos=true;
                                terminarPartidaPorQuinientos(tiradasTotales, ronda, vAcuPuntajeJugador[jugador]);
                                cout<<mJugadores[jugador]<<endl; //ACOMODAR SALIDA DE NOMBRE ######-------#########
                            }
                            else{
                                mostrarPuntajeAcumulado(tiradasTotales, ronda, vAcuPuntajeJugador[jugador]);
                            }
                        }
                        else{
                            /// FINALIZA JUEGO POR SUMAR OBTENER ESCALERA
                            escalera=true;
                            terminarPartidaPorEscalera(mJugadores, vGanadorEscalera, vAcuPuntajeJugador, vTiradasTotales[jugador], ronda, jugador, jugadores);
                            cout<<mJugadores[jugador]<<endl; //ACOMODAR SALIDA DE NOMBRE ######-------#########
                        }
                    }
                    if(ronda==maxRondas){
                        /// SE MUESTRA EL RESULTADO FINAL SI SE LLEGO AL MAXIMO DE RONDAS
                        terminarPartidaPorRondasMaximas(tiradasTotales, ronda, vAcuPuntajeJugador[jugador-1]);
                        cout<<mJugadores[jugador]<<endl; //ACOMODAR SALIDA DE NOMBRE ######-------#########
                    }
                    rlutil::anykey();
                    if(!mute)musicaMenuPrincipal();
                    iniciarPartida=false;
                    opcion=0;
                }
                opcion=0;
                break;
            case 2:
                /// MENU MULTIJUGADOR
                while(opcion != 0){
                rlutil::cls();
                opcion = menuMultijugador();
                    switch(opcion){
                        case 1:
                            /// DOS JUGADORES
                            /// SELECCION DE RONDAS
                            iniciarPartida=true;
                            volverInicio=false;
                            opcionRondas = menuMaxRondas();
                            switch(opcionRondas){
                                case 1:
                                    maxRondas=1;
                                    break;
                                case 2:
                                    maxRondas=5;
                                    break;
                                case 3:
                                    maxRondas=10;
                                    break;
                                case 0:
                                    iniciarPartida=false;
                                    break;
                            }
                            while(iniciarPartida){
                            /// INICIALIZACION DE JUEGO
                                ronda=0, tiradasTotales=0, jugadores=2, cQuinientos=0;
                                escalera=false, quinientos=false;
                                reiniciarPartida(mPuntajeRondaJugador, mPuntajesTiradas, vAcuPuntajeJugador, vTiradasTotales, vGanadorEscalera, vGanadorQuinientos, CANTIDAD_JUGADORES);

                                cargarNombre(mJugadores, 7, jugadores-1); /// SE SOLICITAN DOS NOMBRES

                                if(!mute)playMusicaPartida(); /// MUSICA PARTIDA

                                /// SE JUEGAN LA CANTIDAD MAXIMA DE RONDAS INDICADAS
                                while(ronda<maxRondas && !escalera && !quinientos){
                                    ronda++;
                                    jugarRonda(vDados, mPuntajesTiradas, mPuntajeRondaJugador, vGanadorEscalera, vTiradasTotales, mJugadores, ronda, jugadores);
                                    for(jugador=0;jugador<jugadores;jugador++){
                                        if(!vGanadorEscalera[jugador]){
                                            /// GUARDA Y SE MUESTRA MAXIMO PUNTAJE DE LAS TRES TIRADAS EN LA RONDA
                                            cargarPuntajes(mPuntajeRondaJugador, mPuntajesTiradas, vAcuPuntajeJugador, ronda-1, jugador);
                                            mostrarPuntajeRonda(mPuntajeRondaJugador[jugador][ronda-1], jugador);
                                            if(vAcuPuntajeJugador[jugador]>=500){
                                                /// FINALIZA JUEGO POR SUMAR QUINIENTOS PUNTOS
                                                cQuinientos++;
                                                quinientos=true;
                                                vGanadorQuinientos[jugador]=true;
                                            }
                                            else{
                                                mostrarPuntajeAcumulado(vTiradasTotales[jugador], ronda, vAcuPuntajeJugador[jugador]);
                                            }
                                        }
                                        else{
                                            /// FINALIZA JUEGO POR SUMAR OBTENER ESCALERA
                                            cEscalera++;
                                            escalera=true;
                                        }
                                        if(ronda==maxRondas){
                                            /// SE MUESTRA EL RESULTADO FINAL SI SE LLEGO AL MAXIMO DE RONDAS
                                            terminarPartidaPorRondasMaximas(vTiradasTotales[jugador], ronda, vAcuPuntajeJugador[jugador]);
                                            cout<<mJugadores[jugador]<<endl; //ACOMODAR SALIDA DE NOMBRE ######-------#########
                                        }
                                    }
                                    /// FINALIZAR PARTIDA POR ESCALERA Y VALIDACION PARA DESEMPATE
                                    if(escalera){
                                        if(cEscalera>1){
                                            validarGanador = desempateEscalera(vGanadorEscalera, vAcuPuntajeJugador, vTiradasTotales, jugadores);
                                            terminarPartidaPorEscalera(mJugadores, vGanadorEscalera, vAcuPuntajeJugador, vTiradasTotales[jugador], ronda, validarGanador, jugadores);
                                        }
                                        else{
                                            for(jugador=0;jugador<jugadores;jugador++){
                                                if(vGanadorEscalera[jugador]){
                                                    terminarPartidaPorEscalera(mJugadores, vGanadorEscalera, vAcuPuntajeJugador, vTiradasTotales[jugador], ronda, jugador, jugadores);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    /// FINALIZAR PARTIDA POR QUINIENTOS Y VALIDACION PARA DESEMPATE
                                    else if(quinientos){
                                        if(cQuinientos>1){
                                            terminarPartidaPorQuinientos(vTiradasTotales[jugador], ronda, vAcuPuntajeJugador[jugador]);
                                            cout<<mJugadores[jugador]<<endl; //ACOMODAR SALIDA DE NOMBRE ######-------#########
                                        }
                                        else{
                                            terminarPartidaPorQuinientos(vTiradasTotales[jugador], ronda, vAcuPuntajeJugador[jugador]);
                                            cout<<mJugadores[jugador]<<endl; //ACOMODAR SALIDA DE NOMBRE ######-------#########
                                        }
                                    }
                                    rlutil::anykey();
                                    resultadosRonda(mJugadores, vAcuPuntajeJugador, ronda, jugadores);
                                }

                                if(!mute)musicaMenuPrincipal();
                                iniciarPartida=false;
                                volverInicio=true;
                                opcion=0;
                            }
                            /// SE CAMBIA OPCION A -1 PARA NO CERRAR EL WHILE DE MENU MULTIJUGADOR
                            if(!volverInicio){
                                opcion=-1;
                            }
                            break;
                        case 2:
                            /// TRES JUGADORES
                            break;
                        case 3:
                            /// CUATRO JUGADORES
                            break;
                        case 0:
                            /// VOLVER
                            break;
                    }
                }
                break;
            case 3:
                /// RANKING
                break;
            case 4:
                /// MENU OPCIONES
                while(opcion != 0){
                    rlutil::cls();
                    opcion = menuOpciones();
                    switch(opcion){
                        case 1:
                            /// SONIDO
                            while(opcion != 0){
                                rlutil::cls();
                                opcion = menuSonido();
                                switch(opcion){
                                    case 1:
                                        /// QUITAR SONIDO
                                        mute=true;
                                        stopMusica();
                                        break;
                                    case 2:
                                        /// PONER MUSICA
                                        mute=false;
                                        musicaMenuPrincipal();
                                        break;
                                    case 3:
                                        /// CAMBIAR MUSICA
                                        break;
                                    case 0:
                                        /// VOLVER
                                        break;
                                }
                            }
                            /// SE CAMBIA OPCION A -1 PARA NO CERRAR EL WHILE DE MENU OPCIONES
                            opcion = -1;
                            break;
                        case 2:
                            /// CAMBIAR COLOR
                            while(opcion != 0){
                                rlutil::cls();
                                opcion = menuColor();
                                switch(opcion){
                                    case 1:
                                        ///COLOR AZUL
                                        cambiarColor();
                                        break;
                                    case 2:
                                        ///COLOR VIOLETA
                                        cambiarColor2();
                                        break;
                                    case 3:
                                        ///COLOR NEGRO
                                        cambiarColor3();
                                    case 0:
                                        /// VOLVER
                                        break;
                                }
                            }
                            /// SE CAMBIA OPCION A -1 PARA NO CERRAR EL WHILE DE MENU OPCIONES
                            opcion = -1;
                            break;
                        case 0:
                            /// VOLVER
                            break;
                    }
                }
                break;
            case 0:
                /// CERRAR JUEGO
                return 0;
        }
    }
}
