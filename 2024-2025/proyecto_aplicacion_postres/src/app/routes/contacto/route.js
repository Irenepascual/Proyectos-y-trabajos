// import { NextResponse } from "next/server";
// Verifica que la ruta sea correcta
import db from '@/libs/db'; 

// Manejar la creacion de una nueva consulta en una base de datos en respuesta a una 
// solicitud POST. Si la solicitud contiene todos los datos necesarios (nombre, correo, 
// asunto y duda), se guarda la consulta y se responde con los datos de la nueva consulta. 
// Si faltan datos o ocurre un error durante el proceso, se devuelve un mensaje de 
// error con el codigo de estado correspondiente (400 o 500).

export async function POST(request) {
    try {
        const data = await request.json();

        // Validación de datos
        if (!data.nombre || !data.correo || !data.asunto || !data.duda) {
            console.error("Faltan datos obligatorios:", data);
            return new Response(JSON.stringify({ error: 'Faltan datos obligatorios' }), { status: 400 });
            // return NextResponse.json({ message: 'Faltan datos obligatorios' }, { status: 400 });
        }

        console.log("Datos recibidos para crear consulta:", data);

        // Intentar guardar en la base de datos
        const nuevaConsulta = await db.consulta.create({
            data: {
                nombre: data.nombre,
                correo: data.correo,
                asunto: data.asunto,
                pregunta: data.duda,
                fecha_consulta: new Date(),
                respondida: false,
            },
        });

        return new Response(JSON.stringify(nuevaConsulta), { status: 201 });
        // return NextResponse.json(nuevaConsulta, { status: 201 });

    } catch (error) {
        console.error("Error en el proceso de guardado:", error);
        return new Response(JSON.stringify({ error: 'Error al procesar la solicitud' }), { status: 500 });
        // return NextResponse.json({ message: 'Error al procesar la solicitud' }, { status: 500 });
    }
}
