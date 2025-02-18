#!/bin/bash
#874762, Vayad Díaz, Iván, M, 3, A
#871627, Pascual Albericio, Irene, M, 3, A

eliminarUser() {
        if test $? -eq 0
        then
                if id "$1" &>/dev/null
                then
                        tar -cf "$1.tar" "home/$1" &>/dev/null
                        cp "$1.tar" "/extra/backup"
                        if test $? -eq 0
                        then
                                userdel -r "$1" &>/dev/null
                        fi
                fi
        fi
}

agnadirUser() {
        if id "$1" &>/dev/null
        then
                echo "El usuario $1 ya existe"
        else
                useradd -K PASS_MAX_DAYS=30 -K UID_MIN=1815 -m -k /etc/skel -c "$3" -U "$1"
                echo "$1:$2" | chpasswd
                usermod -U "$1"
                echo "$3 ha sido creado"
        fi
}


if test $(id -u) -ne 0
then
        echo "Este script necesita privilegios de administracion"
else
        if test "$#" -eq 2
        then
                if test "$1" = "-a"
                then
                        OldIFS=$IFS
                        IFS=,
                        cat $2 | while read -r login_name contr nombre_user resto
                        do
                                if [ ! "$login_name" -o ! "$contr" -o ! "$nombre_user" ]
                                then
                                        echo "Campo invalido"
                                        exit 1
                                else
                                        agnadirUser $login_name $contr $nombre_user
                                fi
                        done
                        IFS=$OldIFS
                elif test "$1" = "-s"
                then
                        OldIFS=$IFS
                        IFS=,
                        if test ! -d /extra/backup
                        then
                                mkdir -p /extra/backup &>/dev/null
                        fi
                        cat $2 | while read -r login_name contr nombre_user resto
                        do
                                if [ ! -z "$login_name" ]
                                then
                                        eliminarUser $login_name
                                else
                                        echo "Campo invalido"
                                        exit 1
                                fi
                        done
                        IFS=$OldIFS
                else
                        echo "Opcion invalida" 1>&2
                fi
        else
                echo "Numero incorrecto de parametros" 1>&2
        fi
fi