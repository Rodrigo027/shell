                                                                     
                                                                     
                                                                     
                                             
# Nome: Rodrigo Fernandes da Costa

# n. USP: 6793073

# 

# Nome: Lucas de Moraes Franco

# n. USP: 6793239



main : main.o libbg.so libcd.so libexit.so libfg.so libhistory.so libjobs.so libkill.so libparser.so libpwd.so libquit.so libsystem.so

	gcc -o main main.o -L. -ldl



libbg.so : bg.o

	gcc -shared -fPIC -o libbg.so bg.o



bg.o : bg.c main.h

	gcc -fPIC -c bg.c



libcd.so : cd.o

	gcc -shared -fPIC -o libcd.so cd.o



cd.o : cd.c main.h

	gcc -fPIC -c cd.c



libexit.so : exit.o

	gcc -shared -fPIC -o libexit.so exit.o



exit.o : exit.c main.h

	gcc -fPIC -c exit.c



libfg.so : fg.o

	gcc -shared -fPIC -o libfg.so fg.o



fg.o : fg.c main.h

	gcc -fPIC -c fg.c



libhistory.so : history.o

	gcc -shared -fPIC -o libhistory.so history.o



history.o : history.c main.h

	gcc -fPIC -c history.c



libjobs.so : jobs.o

	gcc -shared -fPIC -o libjobs.so jobs.o



jobs.o : jobs.c main.h

	gcc -fPIC -c jobs.c



libkill.so : kill.o

	gcc -shared -fPIC -o libkill.so kill.o



kill.o : kill.c main.h

	gcc -fPIC -c kill.c



main.o : main.c main.h

	gcc -c main.c



libparser.so : parser.o

	gcc -shared -fPIC -o libparser.so parser.o



parser.o : parser.c main.h

	gcc -fPIC -c parser.c



libpwd.so : pwd.o

	gcc -shared -fPIC -o libpwd.so pwd.o



pwd.o : pwd.c main.h

	gcc -fPIC -c pwd.c



libquit.so : quit.o

	gcc -shared -fPIC -o libquit.so quit.o



quit.o : quit.c main.h

	gcc -fPIC -c quit.c



libsystem.so : system.o

	gcc -shared -fPIC -o libsystem.so system.o



system.o : system.c main.h

	gcc -fPIC -c system.c
