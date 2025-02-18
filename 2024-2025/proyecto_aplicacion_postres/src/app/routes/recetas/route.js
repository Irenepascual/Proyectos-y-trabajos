import db from "@/libs/db";

/*
* Devuelve todas las recetas que están aprobadas
*/
export async function GET(request) {
    const { searchParams } = new URL(request.url);
    const nomReceta = searchParams.get("nom_receta");
    const nomUsuario = searchParams.get("nom_usuario");
    const fecha = searchParams.get("fecha_creac_receta");
    const estado = searchParams.get("estado");

    try {
        let recetas = [];
        let skip = 0;

        if (!estado) {
            if (nomReceta && nomUsuario && fecha) {
                const receta = await db.receta.findFirst({
                    where: {
                        nom_receta: nomReceta,
                        nom_usuario_r: nomUsuario,
                        fecha_creac_receta: fecha,
                    }
                });
                recetas.push(receta);
                skip++;
                return new Response(JSON.stringify(receta), { status: 200 });
            }

            if (nomReceta) {
                while(true){
                    const receta = await db.receta.findFirst({
                        where: {
                            nom_receta: {
                                startsWith: nomReceta,
                                mode: "insensitive"
                            }
                        },
                        skip,
                    });
                    if (!receta) break; 
                    recetas.push(receta);
                    skip++;
                }
                return new Response(JSON.stringify(recetas), { status: 201 });
            }

            if (nomUsuario) {
                while (true) {
                    const receta  = await db.receta.findFirst({
                        where: {
                            nom_usuario_r: nomUsuario,
                        },
                        skip,
                    });
                    if (!receta) break;
                    recetas.push(receta);
                    skip++; 
                }
                return new Response(JSON.stringify(recetas), { status: 202 });
            }

            while (true){
                const receta = await db.receta.findFirst({
                    skip,
                });
                if (!receta) break;
                recetas.push(receta);
                skip++;
            }
            return new Response(JSON.stringify(recetas), { status: 203 });

        } else if (estado == "aprobado") {
            while (true) {
                const receta = await db.receta.findFirst({
                    where: {
                        estado: "aprobado",
                    },
                    skip,
                });
                if (!receta) break;
                recetas.push(receta);
                skip++;
            }
            return new Response(JSON.stringify(recetas), { status: 200 });
        } else if (estado == "no_aprobado") {
            while (true) {
                const receta = await db.receta.findFirst({
                    where: {
                        estado: "no_aprobado",
                    },
                    skip,
                });
                if (!receta) break;
                recetas.push(receta);
                skip++;
            }
            return new Response(JSON.stringify(recetas), { status: 200 });
        } else if (estado == "rechazado") {
            while (true){
                const receta = await db.receta.findFirst({
                    where: {
                        estado: "rechazado",
                    },
                    skip,
                });
                if (!receta) break;
                recetas.push(receta);
                skip++;
            }
            return new Response(JSON.stringify(recetas), { status: 200 });
        }
    } catch (error) {
        console.error("Error al obtener las recetas:", error);
        return new Response(JSON.stringify({ error: "Error al obtener las recetas" }), { status: 500 });
    }
}

/*
* Registran un nueva receta, verificando que no falte
* ningún campo. 
*/
export async function POST(request) {
    try {
        const data = await request.json();
        if (!data.nombreReceta) {
            console.log("Falta el nombre de la receta:", data);
            return new Response(JSON.stringify({ error: "Error: falta el nombre de la receta" }), { status: 400 });
        }
        if (!data.nomUsuario) {
            console.log("Falta el nombre del usuario:", data);
            return new Response(JSON.stringify({ error: "Error: falta el nombre del usuario" }), { status: 400 });
        }
        if (!data.numPersonas) {
            console.log("Falta el número de personas:", data);
            return new Response(JSON.stringify({ error: "Error: falta el número de personas" }), { status: 400 });
        }
        if (!data.tiempo) {
            console.log("Falta el tiempo:", data);
            return new Response(JSON.stringify({ error: "Error: falta el tiempo" }), { status: 400 });
        }
        if (!data.imagenUrl) {
            console.log("Falta la url:", data);
            return new Response(JSON.stringify({ error: "Error: falta la url" }), { status: 400 });
        }
        if (!data.fecha) {
            console.log("Falta la fecha:", data);
            return new Response(JSON.stringify({ error: "Error: falta la fecha" }), { status: 400 });
        }
        if (!data.estado) {
            console.log("Falta el estado:", data);
            return new Response(JSON.stringify({ error: "Error: falta el estado" }), { status: 400 });
        }
        
        const estado = data.estado;

        const nuevaReceta = await db.receta.create({
            data: {
                nom_usuario_r: data.nomUsuario,
                nom_receta: data.nombreReceta,
                fecha_creac_receta: new Date(data.fecha),
                descripcion: data.descripcion,
                foto: data.imagenUrl,
                tiempo_prep: parseInt(data.tiempo,10),
                num_personas: parseInt(data.numPersonas,10),
                estado: estado,
            },
        });
        return new Response(JSON.stringify(nuevaReceta), { status: 200 });
    } catch (error) {
        console.error("Error al crear la receta:", error);
        return new Response(JSON.stringify({ error: "Error al crear la receta" }), { status: 500 });
    }
}

export async function DELETE(request) {
    try {
        const { nom_usuario_r, nom_receta, fecha_creac_receta } = await request.json();
        
        if (!nom_usuario_r || !nom_receta || !fecha_creac_receta) {
            return new Response(JSON.stringify({ error: "Datos incompletos" }), { status: 500 });
        }

        // Eliminar entradas en la tabla `Utiliza`
        await db.utiliza.deleteMany({
            where: {
                nom_usuario_r_ut: nom_usuario_r,
                nom_receta_ut: nom_receta,
                fecha_creac_receta_ut: new Date(fecha_creac_receta)
            }
        });

        // Eliminar entradas en la tabla `Favoritos`
        await db.favoritos.deleteMany({
            where: {
                nom_usuario_r_f: nom_usuario_r,
                nom_receta_f: nom_receta,
                fecha_creac_receta_f: new Date(fecha_creac_receta)
            }
        });

        // Eliminar la receta en sí misma
        const identificador = await db.receta.findFirst({
            where: {
                nom_usuario_r: nom_usuario_r,
                nom_receta: nom_receta,
                fecha_creac_receta: new Date(fecha_creac_receta),
            },
            select: {
              id: true,  // Seleccionamos solo el campo id
            },
        });
        await db.receta.delete({
            where: {
                id: identificador.id,
                // nom_usuario_r: nom_usuario_r,
                // nom_receta: nom_receta,
                // fecha_creac_receta: new Date(fecha_creac_receta),
            }
        });

        return new Response(JSON.stringify({ message: "Receta eliminada correctamente" }), { status: 200 });
    } catch (error) {
        console.error("Error al eliminar receta:", error);
        return new Response(JSON.stringify({ error: "Error al eliminar la receta" }), { status: 500 });
    }
}

/*
* Modifica las recetas ya creadas (puede modificar
* todos los atributos excepto el nombre de la receta,
* el nombre del usuario y la fecha de creación,
* ya que son nuestra clave primaria.)
*/
export async function PUT(request) {
    const { nom_usuario_r, nom_receta, fecha_creac_receta, descripcion, foto, tiempo_prep, num_personas, estado } = await request.json();
    
    if (!nom_usuario_r || !nom_receta || !fecha_creac_receta) {
        return new Response(JSON.stringify({ error: "Datos incompletos" }), { status: 500 });
    }

    try {
        if (!descripcion && !foto && !tiempo_prep && !num_personas && estado) {
            // Convertir la fecha de creación a un objeto Date
            const fechaCreacion = new Date(fecha_creac_receta);
            const identificador = await db.receta.findFirst({
                where: {
                    nom_usuario_r: nom_usuario_r,
                    nom_receta: nom_receta,
                    fecha_creac_receta: fechaCreacion,
                },
                select: {
                  id: true,  // Seleccionamos solo el campo id
                },
            });
            await db.receta.update({
                where: {
                    id: identificador.id,
                    // nom_usuario_r: nom_usuario_r,
                    // nom_receta: nom_receta,
                    // fecha_creac_receta: fechaCreacion,
                },
                data: {
                    estado: estado
                }
            });
            return new Response(JSON.stringify({ message: "Receta aprobada correctamente" }), { status: 200 });
        }
        else if (descripcion && foto && tiempo_prep && num_personas && estado) {
            const identificador = await db.receta.findFirst({
                where: {
                    nom_usuario_r: nom_usuario_r,
                    nom_receta: nom_receta,
                    fecha_creac_receta: fecha_creac_receta,
                },
                select: {
                  id: true,  // Seleccionamos solo el campo id
                },
            });
            
            await db.receta.update({
                where: {
                    id: identificador.id,
                    // nom_usuario_r: nom_usuario_r,
                    // nom_receta: nom_receta,
                    // fecha_creac_receta: fecha_creac_receta,
                },
                data: {
                    descripcion,
                    foto,
                    tiempo_prep: parseInt(tiempo_prep, 10),
                    num_personas: parseInt(num_personas, 10),
                    estado: estado,
                }
            });
            return new Response(JSON.stringify({ message: "Receta modificada correctamente" }), { status: 201 });
        }
        else {
            return new Response(JSON.stringify({ error: "Datos incompletos" }), { status: 500 });
        }

    } catch (error) {
        console.error("Error al modificar receta:", error);
        return new Response(JSON.stringify({ error: "Error al modificar la receta" }), { status: 500 });
    }
}