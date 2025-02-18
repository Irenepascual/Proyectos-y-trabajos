/*
  Warnings:

  - Added the required column `respuesta_rc` to the `Responder_consulta` table without a default value. This is not possible if the table is not empty.

*/
-- AlterTable
ALTER TABLE "Responder_consulta" ADD COLUMN     "respuesta_rc" VARCHAR(255) NOT NULL;
