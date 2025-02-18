import db from "@/libs/db";

// Implementa una API para gestionar recetas favoritas en una base de datos, 
// permitiendo obtener, agregar y eliminar favoritos de los usuarios. El manejo de 
// errores y la validacion de parametros se realiza de manera eficiente, respondiendo 
// con los codigos de estado adecuados y asegurando que las operaciones se realicen 
// correctamente.

export async function GET(request) {
    const { searchParams } = new URL(request.url);
    const nomUsuario = searchParams.get("nom_usuario");
    const nomUsuarioR = searchParams.get("nom_usuario_r");
    const nomReceta = searchParams.get("nom_receta");
    const fecha = searchParams.get("fecha_creac_receta");

    try {
        if (nomUsuario && nomUsuarioR && nomReceta && fecha) {
            const favorito = await db.favoritos.findFirst({
                where: {
                    nom_usuario_u_f: nomUsuario,
                    nom_receta_f: nomReceta,
                    nom_usuario_r_f: nomUsuarioR,
                    fecha_creac_receta_f: fecha,
                }
            });
             // true si hay algun resultado
            const encontrado = !!favorito; 
            return new Response(JSON.stringify(encontrado), { status: 201 });
        }

        if (nomUsuario) {
            const favoritos = [];
            let skip = 0;
            
            while(true){
                const favorito  = await db.favoritos.findFirst({
                    skip,
                    where: {
                        nom_usuario_u_f: nomUsuario,
                    }
                });

                if (!favorito) break;
                favoritos.push(favorito);
                skip++;
            }
            return new Response(JSON.stringify(favoritos), { status: 202 });
        }
        
        console.error("Nombre de usuario no proporcionado");
        return new Response(JSON.stringify({ error: "Nombre de usuario no proporcionado" }), { status: 501 });
    } catch (error) {
        console.error("Error al obtener las recetas favoritas:", error);
        return new Response(JSON.stringify({ error: "Error al obtener las recetas favoritas" }), { status: 500 });
    }
}

export async function POST(request) {
    const { nom_usuario, nom_usuario_r, nom_receta, fecha_creac_receta } = await request.json();

    try {
        const nuevoFavorito = await db.favoritos.create({
            data: {
                nom_usuario_u_f: nom_usuario,
                nom_receta_f: nom_receta,
                nom_usuario_r_f: nom_usuario_r,
                fecha_creac_receta_f: fecha_creac_receta,
            }
        });

        return new Response(JSON.stringify(nuevoFavorito), { status: 201 });
    } catch (error) {
        console.error("Error al agregar la receta a favoritos:", error);
        return new Response(JSON.stringify({ error: "Error al agregar la receta a favoritos" }), { status: 500 });
    }
}

export async function DELETE(request) {
    const { nom_usuario, nom_usuario_r, nom_receta, fecha_creac_receta } = await request.json();

    try {
        await db.favoritos.deleteMany({
            where: {
                nom_usuario_u_f: nom_usuario,
                nom_receta_f: nom_receta,
                nom_usuario_r_f: nom_usuario_r,
                fecha_creac_receta_f: fecha_creac_receta,
            }
        });

        return new Response(JSON.stringify({ message: "Favorito eliminado correctamente" }), { status: 200 });
    } catch (error) {
        console.error("Error al eliminar la receta de favoritos:", error);
        return new Response(JSON.stringify({ error: "Error al eliminar la receta de favoritos" }), { status: 500 });
    }
}



