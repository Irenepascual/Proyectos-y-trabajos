#!/bin/bash

echo "Eliminando el clúster..."
./eliminar_cluster.sh &>/dev/null

echo "Creando el nuevo clúster..."
./kind-with-registry.sh

echo "Eliminando los archivos ejecutables..."
rm Dockerfiles/servidor/servidor
rm Dockerfiles/cliente/cliente

echo "Compilando main del servidor..."
cd cmd/srvraft
CGO_ENABLED=0 go build -o ../../Dockerfiles/servidor/servidor main.go

echo "Compilando main del cliente..."
cd ../cliraft
CGO_ENABLED=0 go build -o ../../Dockerfiles/cliente/cliente main.go


echo "Creando las imágenes Docker..."
cd ../../Dockerfiles/servidor

chmod +x servidor

echo "Creando el contenedor del servidor..."
docker build . -t localhost:5001/servidor:latest

echo "Subiendo el contenedor al repositorio local"
docker push localhost:5001/servidor:latest

cd ../cliente
echo "Creando el contenedor del cliente..."
docker build . -t localhost:5001/cliente:latest

echo "Subiendo el contenedor al repositorio local"
docker push localhost:5001/cliente:latest

echo "Lanzando las Kubernetes..."
cd ../..

kubectl delete statefulset ss &>/dev/null
kubectl delete pod cliente1 &>/dev/null
kubectl delete service ss-service &>/dev/null
kubectl create -f statefulset_go.yaml


echo "Ejecuta el ejecutable en el pod cliente1 con: kubectl exec -it cliente1 -- /bin/sh" 
