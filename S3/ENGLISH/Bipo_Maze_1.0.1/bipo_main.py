# -*- coding: Latin-1 -*-
#-------------------------------------------------------------------------------
# Name:        Bipo Maze
# Author:      Jimmy (WoofWoofDude)
# Created:     2012/04/06
# Python:      3.2.2 (default, Sep  4 2011, 09:51:08) [MSC v.1500 32 bit (Intel)]
#-------------------------------------------------------------------------------
#!/usr/bin/env python

#-----------------------Libs import-----------------------#
from pygame import init, display, Color, key, quit, joystick, time
from pygame.locals import *
from sys import exit
# FIN

#-----------------------Local import-----------------------#
from tygame.main import StaticFrame, Button, Label, render_widgets, handle_widgets #But you can put in ..\Python\Lib\site-packages
from Classe import *

# FIN

def main(difficulty):
    #---------------------Pygame init--------------------#
    init()


    #Création de la fenêtre
    WW, WH = 640, 480
    Window = display.set_mode((WW, WH))

    #icone = image.load("Bipo.png")
    #icone.set_colorkey(const.pink)
    #display.set_icon(icone)
    display.set_caption("Bipo Maze")
    # FIN

    if difficulty == 1:
        taille = 6
    elif difficulty == 2:
        taille = 9
    elif difficulty == 3:
        taille = 12

    #---------------------Typgame--------------------#
    Frame = StaticFrame(Window, colour = const.Pblue, header = False, bordercolor = const.Pgreen, borderwidth = 5, width = 229, height = 470)
    Frame.place((401, 0))
    #---------------------Some variables--------------------#
    mylaby = laby(taille, taille)
    mylaby.generate_laby()
    perso = Perso(mylaby) 
        
    perso_time = 0
    display.flip()
    key.set_repeat(50, 55)
    # FIN


    if joystick.get_count() > 0:
        Joy = joystick.Joystick(0)
        Joy.init()


    while True:
        time.Clock().tick(30)
        Window.fill(const.Porange)    

        for event in handle_widgets():
            if event.type == QUIT:
                quit()
                exit()
                        
            elif event.type == JOYAXISMOTION:
                if event.axis == 1:
                    if round(event.value) < 0:
                        if not perso.che_jaune:
                            perso.move(const.up)
                    if round(event.value) > 0:
                        if not perso.che_jaune:
                            perso.move(const.down)
                if event.axis == 0:
                    if round(event.value) < 0:
                        if not perso.che_jaune:
                            perso.move(const.left)
                    if round(event.value) > 0:
                        if not perso.che_jaune:
                            perso.move(const.right)
            
            
        keys = key.get_pressed()
        if keys:
            if keys[K_UP]:
                if not perso.che_jaune:
                    perso.move(const.up)
            if keys[K_DOWN]:
                if not perso.che_jaune:
                    perso.move(const.down)
            if keys[K_LEFT]:
                if not perso.che_jaune:
                    perso.move(const.left)
            if keys[K_RIGHT]:
                if not perso.che_jaune:
                    perso.move(const.right)
                    

        if time.get_ticks() - perso_time >= const.time_perso_poll:
            perso_time = time.get_ticks()
            perso.poll()


        perso.show(Window)
        render_widgets()
        display.flip()
        
        if perso.x == perso.laby.w - 1 and perso.y == perso.laby.h - 1:
            quit()
            exit()
