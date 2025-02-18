import db from '@/libs/db';
import { NextResponse } from 'next/server';

// Este codigo implementa tres operaciones para gestionar los ingredientes en espera en 
// la base de datos: obtener todos los ingredientes en espera (GET), eliminar un ingrediente 
// específico (DELETE), y aprobar un ingrediente (PUT). Además, realiza validaciones de datos 
// y manejo de errores para garantizar que las operaciones se realicen correctamente y 
// devuelve respuestas adecuadas segun el resultado de cada operacion.

export async function GET() {
    try {
        const ingredientesEnEspera = [];
        let skip = 0;
        while(true){
            const ingrediente  = await db.ingrediente.findFirst({
                skip,
                where: { estado: "no_aprobado" },
            });

            if (!ingrediente) break;
            ingredientesEnEspera.push(ingrediente);
            skip++;
        }
        return NextResponse.json(ingredientesEnEspera);
    } catch (error) {
        console.error("Error al obtener ingredientes en espera:", error);
        return new NextResponse("Error al obtener ingredientes en espera", { status: 500 });
    }
}


export async function DELETE(request) {
    try {
        const { nom_usuario_i, nom_ingred } = await request.json();

        if (!nom_usuario_i || !nom_ingred) {
            return new NextResponse("Datos incompletos", { status: 400 });
        }

        // Verificar si el ingrediente existe antes de intentar eliminar
        const ingredienteExistente = await db.ingrediente.findFirst({
            where: {
                nom_ingred: nom_ingred,
                nom_usuario_i: nom_usuario_i,
            }
        });

        if (!ingredienteExistente) {
            return new NextResponse("Ingrediente no encontrado", { status: 404 });
        }

        // Eliminar todas las restricciones asociadas al ingrediente
        await db.tiene_Restriccion.deleteMany({
            where: {
                nom_usuario_t: nom_usuario_i,
                nom_ingred_t: nom_ingred
            }
        });

        // Eliminar el ingrediente en sí
        const identificador = await db.ingrediente.findFirst({
            where: {
                nom_ingred: nom_ingred,
                nom_usuario_i: nom_usuario_i,
            },
            select: {
              id: true,  // Seleccionamos solo el campo id
            },
        });
        await db.ingrediente.delete({
            where: {
                id: identificador.id,
                // nom_ingred: nom_ingred,
                // nom_usuario_i: nom_usuario_i,
            }
        });

        return new NextResponse("Ingrediente eliminado correctamente", { status: 200 });
    } catch (error) {
        console.error("Error al eliminar ingrediente:", error.message);
        return new NextResponse("Error al eliminar ingrediente", { status: 500 });
    }
}

export async function PUT(request) {
    try {
        const { nom_usuario_i, nom_ingred } = await request.json();

        if (!nom_usuario_i || !nom_ingred) {
            return new NextResponse("Datos incompletos", { status: 400 });
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
                estado: "aprobado"
            }
        });

        return new NextResponse("Ingrediente aprobado correctamente", { status: 200 });
    } catch (error) {
        console.error("Error al aprobar ingrediente:", error);
        return new NextResponse("Error al aprobar ingrediente", { status: 500 });
    }
}
