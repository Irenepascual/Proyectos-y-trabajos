import db from '@/libs/db';

// Maneja las ayudas en una base de datos. Permite obtener todas las ayudas con GET, 
// crear nuevas ayudas con POST y eliminar ayudas especificas con DELETE. Además, 
// implementa manejo de errores para cada uno de los metodos y valida que los campos 
// requeridos esten presentes antes de realizar las operaciones en la base de datos.

export async function GET(request) {
    try {
        // Línea de depuración
        console.log("Intentando obtener ayudas de la base de datos..."); 
        const ayudas = [];
        let skip = 0;  

        while (true) {
            const ayudas = await db.ayuda.findFirst({
                skip,
        
            });

            if (!ayuda) break; 
            ayudas.push(ayuda);
            skip++;
        }


        console.log("Ayudas obtenidas:", ayudas); 
        return new Response(JSON.stringify(ayudas), {
            status: 200,
            headers: { 'Content-Type': 'application/json' },
        });
    } catch (error) {
        console.error('Error al obtener las ayudas', error);
        return new Response(JSON.stringify({ error: 'Error al obtener las ayudas' }), { status: 500 });
    }
}

export async function POST(request) {
    try {
        const { pregunta, nom_usuario_a } = await request.json();
        if (!pregunta || !nom_usuario_a) {
            return new Response(JSON.stringify({ error: 'Campos requeridos faltantes' }), { status: 400 });
        }

        const nuevaAyuda = await db.ayuda.create({
            data: {
                nom_usuario_a,
                pregunta,
            },
        });

        return new Response(JSON.stringify(nuevaAyuda), {
            status: 201,
            headers: { 'Content-Type': 'application/json' },
        });
    } catch (error) {
        console.error('Error al crear la ayuda', error);
        return new Response(JSON.stringify({ error: 'Error al crear la ayuda' }), { status: 500 });
    }
}


// Eliminar una ayuda (DELETE)
export async function DELETE(request) {
    try {
        const { nom_usuario_a, pregunta } = await request.json();

        if (!nom_usuario_a || !pregunta) {
            return new Response(JSON.stringify({ error: 'Campos requeridos faltantes' }), { status: 400 });
        }

        // Eliminar la ayuda usando los campos proporcionados
        const ayudaEliminada = await db.ayuda.deleteMany({
            where: {
                nom_usuario_a,
                pregunta,
            },
        });

        if (ayudaEliminada.count > 0) {
            return new Response(JSON.stringify({ message: 'Ayuda eliminada con éxito' }), { status: 200 });
        } else {
            return new Response(JSON.stringify({ error: 'No se encontró la ayuda a eliminar' }), { status: 404 });
        }
    } catch (error) {
        console.error('Error al eliminar la ayuda', error);
        return new Response(JSON.stringify({ error: 'Error al eliminar la ayuda' }), { status: 500 });
    }
}