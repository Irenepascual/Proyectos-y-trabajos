import db from "@/libs/db";

/*
* Devuelve la lista de los ingredientes que utiliza
* la receta que se le pase. 
* Si no encuentra la receta, devuelve error. 
*/
export async function GET(request) {
    const { searchParams } = new URL(request.url);
    const nomUsuarioR = searchParams.get("nom_usuario_r");
    const nomReceta = searchParams.get("nom_receta");
    const fecha = searchParams.get("fecha_creac_receta");

    try {
        if (nomUsuarioR && nomReceta && fecha) {
            let ingred = [];
            let skip = 0;
            while(true){
                const ingrediente = await db.utiliza.findFirst({
                    where: {
                        nom_usuario_r_ut: nomUsuarioR,
                        nom_receta_ut: nomReceta,
                        fecha_creac_receta_ut: fecha,
                    },
                    skip,
                });
                if (!ingrediente) break;
                ingred.push(ingrediente);
                skip++; 
            }
            return new Response(JSON.stringify(ingred), { status: 200 });
        }
        console.error("Error: faltan parámetros para averiguar los ingredientes de una receta");
        return new Response(JSON.stringify({ error: "Error: faltan parámetros para averiguar los ingredientes de una receta" }), { status: 400 });

    } catch (error) {
        console.error("Error al obtener los ingredientes de la receta:", error);
        return new Response(JSON.stringify({ error: "Error al obtener los ingredientes de la receta" }), { status: 500 });
    }
}

/*
* Añade un ingrediente junto con su cantidad y su tipo,
* para cada receta (identificada por el nombre, la fecha
* y el nombre de usuario)
*/
export async function POST(request) {
    try {
        const data = await request.json();

        // Validación de los datos requeridos
        if (!data.nombreIngrediente || !data.nombreUsuario || !data.cantidad || 
            !data.unidad || !data.fecha || !data.nomUsuario || !data.nombreReceta) {
            console.log("Faltan datos obligatorios:", data);
            return new Response(JSON.stringify({ error: "Error: faltan parámetros" }), { status: 400 });
        }

        // Crear la entrada en la tabla Utiliza
        const nuevaUtiliza = await db.utiliza.create({
            data: {
                nom_usuario_i_ut: data.nombreUsuario,
                nom_ingred_ut: data.nombreIngrediente,
                nom_usuario_r_ut: data.nomUsuario,
                nom_receta_ut: data.nombreReceta,
                fecha_creac_receta_ut: new Date(data.fecha),
                cantidad: parseInt(data.cantidad, 10),
                unidad_med: data.unidad,
            },
        });

        // Respuesta exitosa
        return new Response(JSON.stringify(nuevaUtiliza), { status: 200 });
    } catch (error) {
        console.error("Error al crear la entrada en Utiliza:", error);
        return new Response(JSON.stringify({ error: "Error al crear la entrada en Utiliza" }), { status: 500 });
    }

}

/*
* Modifica un ingrediente junto con su cantidad y su tipo,
* para cada receta (identificada por el nombre, la fecha
* y el nombre de usuario)
*/
export async function PUT(request) {
    const { nombreIngrediente, nombreUsuario, cantidad, unidad, nombreReceta, nomUsuarioReceta, fechaReceta } = await request.json();

    if (!nombreIngrediente || !nombreUsuario || !cantidad || !unidad || !nombreReceta || !nomUsuarioReceta || !fechaReceta) {
        console.error("Error: datos incompletos");
        return new Response(JSON.stringify({ error: "Datos incompletos" }), { status: 500 });
    }

    try {
        // Convertir la fecha de creación a un objeto Date
        const fecha = new Date(fechaReceta);
        
        const identificador = await db.utiliza.findFirst({
            where: {
                nom_usuario_i_ut: nombreUsuario,
                nom_ingred_ut: nombreIngrediente,
                nom_usuario_r_ut: nomUsuarioReceta,
                nom_receta_ut: nombreReceta,
                fecha_creac_receta_ut: fecha,
            },
            select: {
              id: true,  // Seleccionamos solo el campo id
            },
        });
        await db.utiliza.update({
            where: {
                id: identificador.id,
            },
            data: {
                cantidad: parseInt(cantidad, 10),
                unidad_med: unidad,
            }
        });
        return new Response(JSON.stringify({ message: "Ingrediente modificado correctamente" }), { status: 200 });
    } catch (error) {
        console.error("Error al modificar el ingrediente:", error);
        return new Response(JSON.stringify({ error: "Error al modificar el ingrediente" }), { status: 500 });
    }
}