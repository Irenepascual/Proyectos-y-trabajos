# Usar una imagen base de Node.js
FROM node:18-slim

# Instalar OpenSSL y dependencias necesarias para Prisma
RUN apt-get update -y && \
    apt-get install -y openssl libssl-dev && \
    rm -rf /var/lib/apt/lists/*

# Establecer el directorio de trabajo dentro del contenedor
WORKDIR /app

# Copiar los archivos package.json y package-lock.json
COPY package*.json ./

# Instalar dependencias
RUN npm install

# Copiar el resto del código de la aplicación
COPY . .

# Generar archivos de cliente de Prisma
RUN npx prisma generate

# Desactivar ESLint durante la construcción
ENV NEXT_DISABLE_ESLINT=true

# Construir la aplicación Next.js
RUN npm run build -- --no-lint

# Exponer el puerto en el que correrá la aplicación
EXPOSE 3000

# Comando para iniciar la aplicación
CMD ["npm", "start"]
