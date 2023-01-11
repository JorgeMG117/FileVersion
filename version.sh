#!/bin/bash

fichero="prueba"
ficheroAux="aux"
ficherSal="${fichero}_sal"


echo "Compilando el programa"

g++ main.cc -o version -std=c++11 -fmax-errors=1


# Version 0

pruebaV0="la bersion insertar numero\nesta habierta a correciones\ny cambios\n aante todo tipo de \nde \nde fallo"

echo -e $pruebaV0 > $fichero

echo "Se va a guardar la version 0 del fichero $fichero"

./version 1 $fichero 0 &>/dev/null


# Version 1

pruebaV1="sera\nla bersion uno\nesta habierta a cambios\ny correciones\n aante todo tipo \n de fayo"

echo -e $pruebaV1 > $fichero

echo "Se va a guardar la version 1 del fichero $fichero"

./version 1 $fichero 1 &>/dev/null


# Version 2

pruebaV2="Esta sera\nla version dos\nesta abierta a cambios\ny correciones\n ante cualquier tipo \n de fallo"

echo -e $pruebaV2 > $fichero

echo "Se va a guardar la version 2 del fichero $fichero"

./version 1 $fichero 2 &>/dev/null



# Volver a version 1
echo "Se va a volver a la version 1 del fichero $fichero"

echo -e $pruebaV1 > $ficheroAux

./version 2 $fichero 1 &>/dev/null

echo "Mostrando diferencias entre el fichero generado y el original"

diff $ficherSal $ficheroAux


# Volver a version 0
echo "Se va a volver a la version 0 del fichero $fichero"

echo -e $pruebaV0 > $ficheroAux

./version 2 $fichero 0 &>/dev/null

echo "Mostrando diferencias entre el fichero generado y el original"

diff $ficherSal $ficheroAux

rm $ficheroAux $fichero "${fichero}_aux" "${fichero}_ant"

rm version