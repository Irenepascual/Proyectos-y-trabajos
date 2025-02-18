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

-- CreateTable
CREATE TABLE "Usuario_registrado" (
    "nom_usuario_u" VARCHAR(50) NOT NULL,

    CONSTRAINT "Usuario_registrado_pkey" PRIMARY KEY ("nom_usuario_u")
);

-- CreateTable
CREATE TABLE "Ingrediente" (
    "nom_usuario_i" VARCHAR(50) NOT NULL,
    "nom_ingred" VARCHAR(50) NOT NULL,
    "estado" VARCHAR(11) NOT NULL,

    CONSTRAINT "Ingrediente_pkey" PRIMARY KEY ("nom_ingred","nom_usuario_i")
);

-- CreateTable
CREATE TABLE "Receta" (
    "nom_usuario_r" VARCHAR(50) NOT NULL,
    "nom_receta" VARCHAR(100) NOT NULL,
    "fecha_creac_receta" TIMESTAMP(3) NOT NULL,
    "descripcion" VARCHAR(255),
    "foto" VARCHAR(255),
    "tiempo_prep" INTEGER,
    "num_personas" INTEGER,
    "estado" VARCHAR(11) NOT NULL,

    CONSTRAINT "Receta_pkey" PRIMARY KEY ("nom_usuario_r","nom_receta","fecha_creac_receta")
);

-- CreateTable
CREATE TABLE "Restriccion" (
    "nom_restr" VARCHAR(30) NOT NULL,

    CONSTRAINT "Restriccion_pkey" PRIMARY KEY ("nom_restr")
);

-- CreateTable
CREATE TABLE "Tiene_Restriccion" (
    "nom_restr_t" VARCHAR(30) NOT NULL,
    "nom_usuario_t" VARCHAR(50) NOT NULL,
    "nom_ingred_t" VARCHAR(50) NOT NULL,
    "tiene" BOOLEAN NOT NULL,

    CONSTRAINT "Tiene_Restriccion_pkey" PRIMARY KEY ("nom_restr_t","nom_usuario_t","nom_ingred_t")
);

-- CreateTable
CREATE TABLE "Utiliza" (
    "nom_usuario_i_ut" VARCHAR(50) NOT NULL,
    "nom_ingred_ut" VARCHAR(50) NOT NULL,
    "nom_usuario_r_ut" VARCHAR(50) NOT NULL,
    "nom_receta_ut" VARCHAR(100) NOT NULL,
    "fecha_creac_receta_ut" TIMESTAMP(3) NOT NULL,
    "cantidad" INTEGER NOT NULL,
    "unidad_med" VARCHAR(50) NOT NULL,

    CONSTRAINT "Utiliza_pkey" PRIMARY KEY ("nom_usuario_i_ut","nom_ingred_ut","nom_usuario_r_ut","nom_receta_ut","fecha_creac_receta_ut")
);

-- CreateTable
CREATE TABLE "Favoritos" (
    "nom_usuario_u_f" VARCHAR(50) NOT NULL,
    "nom_usuario_r_f" VARCHAR(50) NOT NULL,
    "nom_receta_f" VARCHAR(100) NOT NULL,
    "fecha_creac_receta_f" TIMESTAMP(3) NOT NULL,

    CONSTRAINT "Favoritos_pkey" PRIMARY KEY ("nom_usuario_u_f","nom_usuario_r_f","nom_receta_f","fecha_creac_receta_f")
);

-- CreateTable
CREATE TABLE "Ayuda" (
    "nom_usuario_a" VARCHAR(50) NOT NULL,
    "pregunta" TEXT NOT NULL,

    CONSTRAINT "Ayuda_pkey" PRIMARY KEY ("nom_usuario_a","pregunta")
);

-- CreateTable
CREATE TABLE "Consulta" (
    "correo" VARCHAR(100) NOT NULL,
    "fecha_consulta" TIMESTAMP(3) NOT NULL,
    "nombre" VARCHAR(50),
    "asunto" VARCHAR(100) NOT NULL,
    "pregunta" VARCHAR(255) NOT NULL,
    "respondida" BOOLEAN NOT NULL,

    CONSTRAINT "Consulta_pkey" PRIMARY KEY ("correo","fecha_consulta")
);

-- CreateTable
CREATE TABLE "Responder_consulta" (
    "nom_usuario_rc" VARCHAR(50) NOT NULL,
    "correo_rc" VARCHAR(100) NOT NULL,
    "respuesta_rc" VARCHAR(255) NOT NULL,
    "fecha_consulta_rc" TIMESTAMP(3) NOT NULL,

    CONSTRAINT "Responder_consulta_pkey" PRIMARY KEY ("nom_usuario_rc","correo_rc","fecha_consulta_rc")
);

-- CreateIndex
CREATE UNIQUE INDEX "Usuario_correo_key" ON "Usuario"("correo");

-- CreateIndex
CREATE UNIQUE INDEX "Propietario_codigo_key" ON "Propietario"("codigo");

-- AddForeignKey
ALTER TABLE "Propietario" ADD CONSTRAINT "Propietario_nom_usuario_p_fkey" FOREIGN KEY ("nom_usuario_p") REFERENCES "Usuario"("nom_usuario") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Usuario_registrado" ADD CONSTRAINT "Usuario_registrado_nom_usuario_u_fkey" FOREIGN KEY ("nom_usuario_u") REFERENCES "Usuario"("nom_usuario") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Ingrediente" ADD CONSTRAINT "Ingrediente_nom_usuario_i_fkey" FOREIGN KEY ("nom_usuario_i") REFERENCES "Usuario"("nom_usuario") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Receta" ADD CONSTRAINT "Receta_nom_usuario_r_fkey" FOREIGN KEY ("nom_usuario_r") REFERENCES "Usuario"("nom_usuario") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Tiene_Restriccion" ADD CONSTRAINT "Tiene_Restriccion_nom_restr_t_fkey" FOREIGN KEY ("nom_restr_t") REFERENCES "Restriccion"("nom_restr") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Tiene_Restriccion" ADD CONSTRAINT "Tiene_Restriccion_nom_usuario_t_nom_ingred_t_fkey" FOREIGN KEY ("nom_usuario_t", "nom_ingred_t") REFERENCES "Ingrediente"("nom_usuario_i", "nom_ingred") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Utiliza" ADD CONSTRAINT "Utiliza_nom_usuario_i_ut_nom_ingred_ut_fkey" FOREIGN KEY ("nom_usuario_i_ut", "nom_ingred_ut") REFERENCES "Ingrediente"("nom_usuario_i", "nom_ingred") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Utiliza" ADD CONSTRAINT "Utiliza_nom_usuario_r_ut_nom_receta_ut_fecha_creac_receta__fkey" FOREIGN KEY ("nom_usuario_r_ut", "nom_receta_ut", "fecha_creac_receta_ut") REFERENCES "Receta"("nom_usuario_r", "nom_receta", "fecha_creac_receta") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Favoritos" ADD CONSTRAINT "Favoritos_nom_usuario_u_f_fkey" FOREIGN KEY ("nom_usuario_u_f") REFERENCES "Usuario_registrado"("nom_usuario_u") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Favoritos" ADD CONSTRAINT "Favoritos_nom_usuario_r_f_nom_receta_f_fecha_creac_receta__fkey" FOREIGN KEY ("nom_usuario_r_f", "nom_receta_f", "fecha_creac_receta_f") REFERENCES "Receta"("nom_usuario_r", "nom_receta", "fecha_creac_receta") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Ayuda" ADD CONSTRAINT "Ayuda_nom_usuario_a_fkey" FOREIGN KEY ("nom_usuario_a") REFERENCES "Propietario"("nom_usuario_p") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Responder_consulta" ADD CONSTRAINT "Responder_consulta_nom_usuario_rc_fkey" FOREIGN KEY ("nom_usuario_rc") REFERENCES "Propietario"("nom_usuario_p") ON DELETE RESTRICT ON UPDATE CASCADE;

-- AddForeignKey
ALTER TABLE "Responder_consulta" ADD CONSTRAINT "Responder_consulta_correo_rc_fecha_consulta_rc_fkey" FOREIGN KEY ("correo_rc", "fecha_consulta_rc") REFERENCES "Consulta"("correo", "fecha_consulta") ON DELETE RESTRICT ON UPDATE CASCADE;
