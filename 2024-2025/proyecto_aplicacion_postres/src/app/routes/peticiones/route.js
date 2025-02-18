import db from '@/libs/db';
import nodemailer from 'nodemailer';

/*
* Sirve para tener el correo y la contraseña 
* obtenida en la verificación en 2 pasos, para
* poder contactar con el correo dado.
*/
const transporter = nodemailer.createTransport({
    service: "gmail",
    auth: {
        user: "postrecicoszgz@gmail.com",
        pass: "VACIO, AQUI VA LA CONTRASEÑA" // Se recomienda usar variables de entorno en producción por seguridad
    },
});

/*
* Devuelve todas las consultas no respondidas. 
*/
export async function GET(request) {
    try {

        const dudasNoRespondidas = [];
        let skip = 0;

        while(true){
            const duda  = await db.consulta.findFirst({
                where: { respondida: false },
                skip,
                select: {
                    asunto: true,
                    pregunta: true,
                    fecha_consulta: true,
                    nombre: true,
                    correo: true,  
                },
            });
            if (!duda) break;
            dudasNoRespondidas.push(duda);
            skip++;
        }
        return new Response(JSON.stringify(dudasNoRespondidas), {
            status: 200,
            headers: { "Content-Type": "application/json" },
        });
    } catch (error) {
        console.error("Error al obtener dudas no respondidas:", error);
        return new Response(JSON.stringify({ error: "Error al obtener dudas no respondidas" }), {
            status: 500,
            headers: { "Content-Type": "application/json" },
        });
    }
}

/*
* Registra en la base de datos la respuesta del 
* propietario a la consulta seleccionada, y envía
* un correo con los atributos registrados de asunto
* para el asunto del correo; y repuesta, para el 
* mensaje del correo. 
*/
export async function POST(request) {
    try {
        const data = await request.json();

        // Marcar la consulta como respondida
        const identificador = await db.consulta.findFirst({
            where: {
                correo: data.correo,
                fecha_consulta: new Date(data.fecha_consulta),
            },
            select: {
              id: true,  // Seleccionamos solo el campo id
            },
        });
        await db.consulta.update({
            where: {
                id: identificador.id,
            },
            data: {
                respondida: true,
            },
        });
        console.log("Consulta marcada como respondida");

        // Enviar correo de confirmación
        await transporter.sendMail({
            from: '"Postrecicos" <postrecicoszgzOFC@gmail.com>',
            to: data.correo,
            subject: "Consulta respondida",
            text: `Estimado/a usuario,\n\nSu consulta ha sido respondida:\n\n"${data.respuesta}"\n\nSaludos,\nEquipo de Postrecicos`,
        });
        console.log("Correo de respuesta enviado a:", data.correo);

        return new Response(JSON.stringify({ message: "Correo enviado correctamente" }), {
            status: 200,
            headers: { "Content-Type": "application/json" },
        });
    } catch (error) {
        console.error("Error al responder consulta:", error);

        return new Response(JSON.stringify({ error: "Error al responder consulta", details: error.message }), {
            status: 500,
            headers: { "Content-Type": "application/json" },
        });
    }
}

