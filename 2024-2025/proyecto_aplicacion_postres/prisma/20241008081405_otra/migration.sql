/*
  Warnings:

  - You are about to drop the `usuario` table. If the table is not empty, all the data it contains will be lost.

*/
-- DropTable
DROP TABLE "usuario";

-- CreateTable
CREATE TABLE "Usuario" (
    "nom_usuario" VARCHAR(50) NOT NULL,
    "correo" VARCHAR(100) NOT NULL,
    "contrasena" VARCHAR(50) NOT NULL,

    CONSTRAINT "Usuario_pkey" PRIMARY KEY ("nom_usuario")
);

-- CreateTable
CREATE TABLE "Propietario" (
    "nom_usuario_p" VARCHAR(50) NOT NULL,
    "codigo" INTEGER NOT NULL,

    CONSTRAINT "Propietario_pkey" PRIMARY KEY ("nom_usuario_p")
);

-- CreateIndex
CREATE UNIQUE INDEX "Usuario_correo_key" ON "Usuario"("correo");

-- CreateIndex
CREATE UNIQUE INDEX "Propietario_codigo_key" ON "Propietario"("codigo");

-- AddForeignKey
ALTER TABLE "Propietario" ADD CONSTRAINT "Propietario_nom_usuario_p_fkey" FOREIGN KEY ("nom_usuario_p") REFERENCES "Usuario"("nom_usuario") ON DELETE RESTRICT ON UPDATE CASCADE;
