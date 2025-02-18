import db from '@/libs/db';
import { NextResponse } from 'next/server';

// Implementa dos operaciones importantes para gestionar los ingredientes en una 
// base de datos: obtener los ingredientes aprobados y eliminar un ingrediente junto 
// con sus restricciones asociadas. La operacion GET ordena los ingredientes por el numero 
// de restricciones, mientras que la operacion DELETE elimina tanto las restricciones 
// como el ingrediente de la base de datos. Ademas, el codigo maneja errores adecuadamente 
// y proporciona respuestas con los codigos de estado HTTP apropiados.

export async function GET() {
    try {
        // Obtener ingredientes aprobados, ordenados por el número de restricciones

        const ingredientesAprobados = [];
        let skip = 0; 

        while(true){
            const ingrediente = await db.ingrediente.findFirst({
                where: { estado: "aprobado" },
                skip,
                include: { tiene_Restr: { include: { restriccion: true } } },
                orderBy: {
                    tiene_Restr: {
                        // Ordenar por el número de restricciones, de mayor a menor
                        _count: 'desc' 
                    }
                }
            });

            if (!ingrediente) break;
            ingredientesAprobados.push(ingrediente);
            skip++;
        }
        return NextResponse.json(ingredientesAprobados);
    } catch (error) {
        console.error("Error al obtener ingredientes aprobados:", error);
        return new NextResponse("Error al obtener ingredientes aprobados", { status: 500 });
    }
}

export async function DELETE(request) {
    try {
        const { nom_usuario_i, nom_ingred } = await request.json();

        if (!nom_usuario_i || !nom_ingred) {
            return new NextResponse("Datos incompletos", { status: 400 });
        }

        // Eliminar restricciones asociadas al ingrediente
        await db.tiene_Restriccion.deleteMany({
            where: {
                nom_usuario_t: nom_usuario_i,
                nom_ingred_t: nom_ingred
            }
        });

        // Eliminar el ingrediente en sí
        await db.ingrediente.delete({
            where: {
                nom_ingred: nom_ingred,
                nom_usuario_i: nom_usuario_i,
            }
        });

        return new NextResponse("Ingrediente eliminado correctamente", { status: 200 });
    } catch (error) {
        console.error("Error al eliminar ingrediente:", error);
        return new NextResponse("Error al eliminar ingrediente", { status: 500 });
    }
}
