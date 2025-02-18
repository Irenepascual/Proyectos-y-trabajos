import db from "@/libs/db";

// Implementa una API para gestionar ingredientes en una base de datos, permitiendo obtener 
// ingredientes aprobados, crear nuevos ingredientes, aprobar ingredientes y eliminar ingredientes 
// junto con sus restricciones asociadas. Se incluye manejo de errores y validaciones para 
// garantizar que las operaciones se realicen correctamente y que se devuelvan respuestas 
// adecuadas segun el resultado de cada operacion.

export async function GET(request) {
    const { searchParams } = new URL(request.url);
    const estado = searchParams.get("estado");

    try {
        if (!estado) {
            console.log("NO HAY ESTADO");

            const ingredientes = [];
            let skip = 0;

            while(true){
                const ingrediente = await db.ingrediente.findFirst({
                    skip,
                    where: {
                        estado: "aprobado",
                    },
                    orderBy: {
                        nom_ingred: 'asc',
                    },
                });

                if (!ingrediente) break;

                ingredientes.push(ingrediente);
                skip++;
            }
            return new Response(JSON.stringify(ingredientes), { status: 200 });    
        }
        else if (estado == "aprobado") {
            const ingredientes = [];
            let skip = 0;

            while(true){
                const ingrediente = await db.ingrediente.findFirst({
                    skip,
                    where: { estado: "aprobado" },
                    orderBy: {
                        nom_ingred: 'asc', // Ordenar alfabéticamente por nombre
                    },
                });
                if (!ingrediente) break;

                ingredientes.push(ingrediente);
                skip++;
            }
            return new Response(JSON.stringify(ingredientes), { status: 201 });                
        }
        else if (estado == "no_aprobado") {
            const ingredientes = [];
            let skip = 0;

            while(true){
                const ingrediente = await db.ingrediente.findFirst({
                    skip, 
                    where: { estado: "no_aprobado" },
                    orderBy: {
                        nom_ingred: 'asc', // Ordenar alfabéticamente por nombre
                    },
                });
                ingredientes.push(ingrediente);
                skip++;
            }
            return new Response(JSON.stringify(ingredientes), { status: 202 });                
        }
        else if (estado == "rechazado") {
            const ingredientes = [];
            let skip = 0;

            while(true){
                const ingrediente = await db.ingrediente.findFirst({
                    skip,
                    where: { estado: "rechazado" },
                    orderBy: {
                        nom_ingred: 'asc', // Ordenar alfabéticamente por nombre
                    },
                });
                ingredientes.push(ingrediente);
                skip++;
            }
            return new Response(JSON.stringify(ingredientes), { status: 202 });                
        }
        else {
            console.error("Error en los parámetros al obtener los ingredientes", error);
            return new Response(JSON.stringify({ error: "Error en los parámetros al obtener los ingredientes" }), { status: 400 });
        }
    } catch (error) {
        console.error("Error al obtener los ingredientes:", error);
        return new Response(JSON.stringify({ error: "Error al obtener los ingredientes" }), { status: 500 });
    }
}


export async function PUT(request) {
    try {
        const { nom_usuario_i, nom_ingred, estado } = await request.json();

        if (!nom_usuario_i || !nom_ingred) {
            return new Response(JSON.stringify({ error: "Datos incompletos" }), { status: 400 });
        }

        const identificador = await db.ingrediente.findFirst({
            where: {
                nom_ingred: nom_ingred,
                nom_usuario_i: nom_usuario_i,
            },
            select: {
              id: true,  // Seleccionamos solo el campo id
            },
          });

        await db.ingrediente.update({
            where: {
                id: identificador.id,
            },
            data: {
                estado: estado
            }
        });

        return new Response(JSON.stringify({ message: "Ingrediente aprobado correctamente" }), { status: 200 });
    } catch (error) {
        console.error("Error al aprobar ingrediente:", error);
        return new Response(JSON.stringify({ error: "Error al aprobar el ingrediente" }), { status: 500 });
    }
}


export async function DELETE(request) {
    try {
        const { nom_usuario_i, nom_ingred } = await request.json();

        if (!nom_usuario_i || !nom_ingred) {
            return new Response(JSON.stringify({ error: "Datos incompletos" }), { status: 400 });
        }
     

        const identificadorI = await db.ingrediente.findFirst({
            where: {
                nom_ingred: nom_ingred,
                nom_usuario_i: nom_usuario_i,
            },
            select: {
              id: true,  // Seleccionamos solo el campo id
            },
        });
        
        // Eliminar el ingrediente en sí
        await db.ingrediente.delete({
            where: {
                id: identificadorI.id,
            }
        });

        return new Response(JSON.stringify({ message: "Ingrediente eliminado correctamente" }), { status: 200 });
    } catch (error) {
        console.error("Error al eliminar ingrediente:", error);
        return new Response(JSON.stringify({ error: "Error al eliminar el ingrediente" }), { status: 500 });
    }
}



export async function POST(request) {
    try {
        const data = await request.json();
        const { nom_usuario, nom_ingred, estado } = data;
        console.log("Dentro de POST(): data", data);

        const ingredienteExists = await db.ingrediente.findFirst({
            where: {
                nom_usuario_i: nom_usuario,
                nom_ingred: nom_ingred,
            }
        });

        if (ingredienteExists) {
            return new Response(JSON.stringify({ 
                error: 'El ingrediente ya existe para este usuario' }), {
                status: 400,
              });
        }

        await db.ingrediente.create({
            data: {
                nom_usuario_i: nom_usuario,
                nom_ingred: nom_ingred,
                estado: estado,
            }
        });

        console.log("INGREDIENTE AÑADIDO OKEY");

        return new Response(JSON.stringify({ success: true }), { status: 200 });
    } catch (error) {
        console.error("Error al añadir ingrediente:", error);
        return new Response(JSON.stringify({ error: "Error al añadir el ingrediente" }), { status: 500 });
    }
}