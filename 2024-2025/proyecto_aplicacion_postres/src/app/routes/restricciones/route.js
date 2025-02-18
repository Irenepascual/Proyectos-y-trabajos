import db from "@/libs/db";

/*
* Devuelve todas las restricciones que existen en la base de datos
* (lactosa, sin alcohol, sin gluten...)
*/
export async function GET() {
    try {
        // Obtener todas las restricciones de la base de datos
        let restricciones = [];
        let skip = 0;
        while(true){
            const restriccion = await db.Restriccion.findFirst({
                select: {
                    nom_restr: true
                },
                skip,
            });
            if (!restriccion) break; 
            restricciones.push(restriccion);
            skip++; 
        }

        // Devolver la lista de restricciones en formato JSON
        return new Response(JSON.stringify(restricciones), { status: 200 });
    } catch (error) {
        console.error("Error al obtener restricciones:", error);
        return new Response(JSON.stringify({ error: "Error al obtener restricciones" }), { status: 500 });
    }
}
