export async function POST(request) {
    try {
        const data = await request.json();

        // Almacenar la respuesta en la base de datos
        await db.responder_consulta.create({
            data: {
                correo_rc: data.correo,
                fecha_consulta_rc: new Date(data.fecha_consulta),
                respuesta_rc: data.respuesta,
                nom_usuario_rc: data.nom_usuario_rc,
            },
        });
        console.log("Respuesta almacenada en responder_consulta");


        return new Response(JSON.stringify({ message: "Respuesta enviada correctamente" }), {
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