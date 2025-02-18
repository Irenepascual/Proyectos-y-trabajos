import db from '@/libs/db';
import { NextResponse } from 'next/server';

// Manejar tres operaciones para gestionar recetas en espera en la base de datos: obtener 
// recetas en espera (GET), eliminar una receta especifica (DELETE), y aprobar una receta (PUT). 
// Ademas, se implementan validaciones y manejo de errores para asegurar que las operaciones se 
// realicen correctamente, proporcionando respuestas adecuadas segun el resultado de cada operacion.

export async function GET() {
    try {
        const recetasEnEspera = [];
        let skip = 0;

        while(true){
            const receta = await db.receta.findFirst({
                skip,
                where: { estado: "no_aprobado" }
            });

            if (!receta) break;
            recetasEnEspera.push(receta);
            skip++;
        }
        return NextResponse.json(recetasEnEspera);
    } catch (error) {
        console.error("Error al obtener recetas en espera:", error);
        return new NextResponse("Error al obtener recetas en espera", { status: 500 });
    }
}


export async function DELETE(request) {
    try {
        const { nom_usuario_r, nom_receta, fecha_creac_receta } = await request.json();

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

        return new NextResponse("Receta eliminada correctamente", { status: 200 });
    } catch (error) {
        console.error("Error al eliminar receta:", error);
        return new NextResponse("Error al eliminar receta", { status: 500 });
    }
}


// Aceptar (aprobar) una receta
export async function PUT(request) {
    const { nom_usuario_r, nom_receta, fecha_creac_receta } = await request.json();
    
    if (!nom_usuario_r || !nom_receta || !fecha_creac_receta) {
        return new NextResponse("Datos incompletos", { status: 400 });
    }

    try {
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
                estado: "aprobado"
            }
        });

        return new NextResponse("Receta aprobada correctamente", { status: 200 });
    } catch (error) {
        console.error("Error al aprobar receta:", error);
        return new NextResponse("Error al aprobar receta", { status: 500 });
    }
}
