C:\develop\gbdk\bin\lcc -Wa-l -Wf-ba0 -c -o saveslot1.o saves/saveslot1.c
C:\develop\gbdk\bin\lcc -Wa-l -Wf-bo5 -c -o title.o title_screen/titledata.s
C:\develop\gbdk\bin\lcc -Wa-l -Wf-bo2 -c -o charas.o charas/charas.s
C:\develop\gbdk\bin\lcc -Wa-l -Wf-bo3 -c -o bg.o background/bg.s
C:\develop\gbdk\bin\lcc -Wa-l -Wf-bo4 -c -o fullos.o music/fullos.s
C:\develop\gbdk\bin\lcc -Wa-l -c -o title_screen.o title_screen/title_screen.c
C:\develop\gbdk\bin\lcc -Wa-l -Wf-bo6 -c -o music.o utils/LP1.297a.c

C:\develop\gbdk\bin\lcc -Wa-l -c -o chara_utils.o gbdk-lib-extension/chara_utils.c
C:\develop\gbdk\bin\lcc -Wa-l -c -o rle_lib.o gbdk-lib-extension/rle_lib.c

C:\develop\gbdk\bin\lcc -Wa-l -c -o main.o main.c
C:\develop\gbdk\bin\lcc -Wl-yt3 -Wl-yo8 -Wl-ya4 -o doctorhow.gb main.o title.o title_screen.o charas.o bg.o fullos.o saveslot1.o music.o chara_utils.o rle_lib.o
pause
del *.o
del *.lst
