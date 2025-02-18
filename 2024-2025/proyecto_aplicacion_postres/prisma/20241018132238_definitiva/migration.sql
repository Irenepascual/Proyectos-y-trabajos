/*
  Warnings:

  - The primary key for the `Receta` table will be changed. If it partially fails, the table could be left without primary key constraint.
  - The primary key for the `Utiliza` table will be changed. If it partially fails, the table could be left without primary key constraint.
  - Added the required column `fecha_creac_receta` to the `Receta` table without a default value. This is not possible if the table is not empty.

*/
-- DropForeignKey
ALTER TABLE "Favoritos" DROP CONSTRAINT "Favoritos_nom_usuario_r_f_nom_receta_f_fkey";

-- DropForeignKey
ALTER TABLE "Utiliza" DROP CONSTRAINT "Utiliza_nom_usuario_r_ut_nom_receta_ut_fkey";

-- AlterTable
ALTER TABLE "Receta" DROP CONSTRAINT "Receta_pkey",
ADD COLUMN     "fecha_creac_receta" TIMESTAMP(3) NOT NULL,
ADD CONSTRAINT "Receta_pkey" PRIMARY KEY ("nom_usuario_r", "nom_receta", "fecha_creac_receta");

-- AlterTable
ALTER TABLE "Utiliza" DROP CONSTRAINT "Utiliza_pkey",
ADD CONSTRAINT "Utiliza_pkey" PRIMARY KEY ("nom_usuario_i_ut", "nom_ingred_ut", "nom_usuario_r_ut", "nom_receta_ut", "fecha_creac_receta_ut");

-- AddForeignKey
ALTER TABLE "Utiliza" ADD CONSTRAINT "Utiliza_nom_usuario_r_ut_nom_receta_ut_fecha_creac_receta__fkey" FOREIGN KEY ("nom_usuario_r_ut", "nom_receta_ut", "fecha_creac_receta_ut") REFERENCES "Receta"("nom_usuario_r", "nom_receta", "fecha_creac_receta") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Favoritos" ADD CONSTRAINT "Favoritos_nom_usuario_r_f_nom_receta_f_fecha_creac_receta__fkey" FOREIGN KEY ("nom_usuario_r_f", "nom_receta_f", "fecha_creac_receta_f") REFERENCES "Receta"("nom_usuario_r", "nom_receta", "fecha_creac_receta") ON DELETE RESTRICT ON UPDATE CASCADE;
