import db from "@/libs/db";

/*
* Devuelve las restricciones que tiene un ingrediente
* (identificado por el nombre del ingrediente y nombre
* del creador).
*/
export async function GET(request) {
    const { searchParams } = new URL(request.url);
    const nomIngred = searchParams.get("nom_ingred");
    const nomUsuario = searchParams.get("nom_usuario");

    try {
        // Verificar que ambos parámetros estén presentes
        if (nomUsuario && nomIngred) {
            const restricciones = await db.Tiene_Restriccion.findFirst({
                where: {
                    nom_usuario_t: nomUsuario,
                    nom_ingred_t: nomIngred,
                    tiene: true, // Solo restricciones activas
                },
            });
            if (restricciones) {
                return new Response(JSON.stringify(restricciones), { status: 200 });
            }

            return new Response(JSON.stringify(null), { status: 201 });
        }

        // Si faltan los parámetros requeridos
        return new Response(JSON.stringify({ error: "Parámetros 'nom_usuario' y 'nom_ingred' son requeridos" }), { status: 400 });
        
    } catch (error) {
        console.error("Error al obtener restricciones:", error);
        return new Response(JSON.stringify({ error: "Error al obtener las restricciones" }), { status: 500 });
    }
}


export async function POST(request) {
    try {
        const { nom_usuario_t, nom_ingred_t, restricciones } = await request.json();

        // Insertar las restricciones asociadas
        if (restricciones && restricciones.length > 0) {
            for (const r of restricciones) {
                console.log(nom_usuario_t, nom_ingred_t, r);

                await db.Tiene_Restriccion.create({
                    data: {
                        nom_restr_t: r,
                        nom_usuario_t: nom_usuario_t,
                        nom_ingred_t: nom_ingred_t,
                        tiene: true,
                    }
                });
                // await db.tiene_restriccion.create({
                //     data: {
                //         nom_restr_t: r,
                //         nom_usuario_t: nom_usuario,
                //         nom_ingred_t: nom_ingred,
                //         tiene: true
                //     }
                // });
            }
        }
        return new Response(JSON.stringify({ success: true }), { status: 200 });
    } catch (error) {
        console.error("Error al añadir la restricción del ingrediente:", error);
        return new Response(JSON.stringify({ error: "Error al añadir la restricción del ingrediente" }), { status: 500 });
    }
}


export async function DELETE(request) {
    try {
        const { nom_usuario_i, nom_ingred } = await request.json();

        if (!nom_usuario_i || !nom_ingred) {
            return new Response(JSON.stringify({ error: "Datos incompletos" }), { status: 400 });
        }

        // Eliminar restricciones asociadas al ingrediente
        const identificador = await db.tiene_restriccion.findFirst({
            where: {
                nom_usuario_t: nom_usuario_i,
                nom_ingred_t: nom_ingred,
            },
            select: {
              id: true,  // Seleccionamos solo el campo id
            },
        });

        await db.tiene_restriccion.deleteMany({
            where: {
                id: identificador.id,
            }
        });


        return new Response(JSON.stringify({ message: "Restricciones eliminadas correctamente" }), { status: 200 });
    } catch (error) {
        console.error("Error al eliminar las restricciones:", error);
        return new Response(JSON.stringify({ error: "Error al eliminar las restricciones" }), { status: 500 });
    }
}