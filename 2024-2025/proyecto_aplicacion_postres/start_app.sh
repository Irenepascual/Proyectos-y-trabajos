#!/bin/bash

# Nombre del contenedor
CONTAINER_NAME="postrecicos-container"
IMAGE_NAME="postrecicos:app"

# Detener y eliminar el contenedor anterior si existe
if docker ps -q -f name=$CONTAINER_NAME > /dev/null; then
    echo "Deteniendo el contenedor '$CONTAINER_NAME'..."
    docker stop $CONTAINER_NAME
    echo "Eliminando el contenedor '$CONTAINER_NAME'..."
    docker rm $CONTAINER_NAME
fi

# Construir la imagen
echo -e "\nConstruyendo la imagen '$IMAGE_NAME'..."
docker build -t $IMAGE_NAME .

# Ejecutar el contenedor
echo -e "\nIniciando el contenedor '$CONTAINER_NAME'..."
docker run -d --name $CONTAINER_NAME -p 3000:3000 --env-file .env $IMAGE_NAME

# Verificar que el contenedor está corriendo
if docker ps -q -f name=$CONTAINER_NAME > /dev/null; then
    echo "Contenedor '$CONTAINER_NAME' está corriendo."
else
    echo "Error al iniciar el contenedor '$CONTAINER_NAME'."
    exit 1
fi

# Mostrar información del contenedor
echo -e "\nInformación del contenedor:"
docker ps -f name=$CONTAINER_NAME --format "table {{.ID}}\t{{.Image}}\t{{.Command}}\t{{.CreatedAt}}\t{{.Status}}\t{{.Ports}}\t{{.Names}}"

# Mostrar puertos expuestos
echo -e "\nPuertos expuestos:"
docker port $CONTAINER_NAME

# Obtener la IP pública
IP_PUBLICA=$(curl -s ifconfig.me)
echo -e "\nTu IP pública es: $IP_PUBLICA"

# Mostrar enlace accesible desde fuera de tu red local
echo -e "\nAccede a tu aplicación desde fuera de tu red local usando este enlace:"
echo "http://$IP_PUBLICA:3000"

# Información adicional
echo -e "\nPara hacer que tu aplicación sea accesible desde fuera de tu red local, asegúrate de haber configurado el redireccionamiento de puertos (port forwarding) en tu router."
echo "El puerto 3000 de tu red local debe estar redirigido a tu máquina local con IP $IP_PUBLICA."
echo "Si no lo has hecho, sigue los pasos de tu router para configurarlo."
