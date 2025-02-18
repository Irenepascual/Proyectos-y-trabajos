import { NextResponse } from "next/server";
import db from '@/libs/db';

// Manejar la eliminacion de un usuario en una aplicacion. Antes de eliminar al usuario, 
// reasigna todos sus datos (como ingredientes, recetas, restricciones, ayudas, etc.) al usuario principal 
// "USER". Luego, elimina todas las entradas relacionadas y el registro del usuario en la base de datos. 
// Si todo es exitoso, responde con un mensaje de exito, y si ocurre un error, responde con un mensaje de error.

export async function DELETE(request) {
    try {
        const { nom_usuario } = await request.json();

        if (!nom_usuario) {
            return NextResponse.json({ message: 'Falta el nombre de usuario' }, { status: 400 });
        }

        // Verificar si el usuario existe
        const usuario = await db.usuario.findFirst({
            where: { nom_usuario }
        });

        if (!usuario) {
            return NextResponse.json({ message: 'No se encontró el usuario' }, { status: 404 });
        }

        // Obtener el usuario "USER" para usarlo en las actualizaciones
        const superUsuario = await db.usuario.findFirst({
            where: { nom_usuario: "USER" }
        });

        if (!superUsuario) {
            return NextResponse.json({ message: 'No se encontró el usuario "USER"' }, { status: 500 });
        }

        const superUsuarioNombre = superUsuario.nom_usuario;

        // Actualizar la tabla `Ingrediente` para asignar al usuario "USER"
        await db.ingrediente.updateMany({
            where: { nom_usuario_i: nom_usuario },
            data: { nom_usuario_i: superUsuarioNombre }
        });

        // Actualizar la tabla `Receta` para asignar al usuario "USER"
        await db.receta.updateMany({
            where: { nom_usuario_r: nom_usuario },
            data: { nom_usuario_r: superUsuarioNombre }
        });

        // Actualizar la tabla `Tiene_Restriccion` para asignar al usuario "USER"
        await db.tiene_Restriccion.updateMany({
            where: { nom_usuario_t: nom_usuario },
            data: { nom_usuario_t: superUsuarioNombre }
        });

        // Actualizar la tabla `Utiliza` para asignar al usuario "USER"
        await db.utiliza.updateMany({
            where: { nom_usuario_i_ut: nom_usuario },
            data: { nom_usuario_i_ut: superUsuarioNombre }
        });

        // Eliminar entradas de la tabla `Favoritos`
        await db.favoritos.deleteMany({
            where: { nom_usuario_u_f: nom_usuario }
        });

        // Eliminar el usuario registrado
        await db.usuario_registrado.deleteMany({
            where: { nom_usuario_u: nom_usuario }
        });

        // Eliminar el usuario principal
        await db.usuario.delete({
            where: { nom_usuario }
        });

        return NextResponse.json({ message: 'Cuenta eliminada correctamente' }, { status: 200 });

    } catch (error) {
        console.error("Error al eliminar la cuenta:", error);
        return NextResponse.json({ message: 'Error al eliminar la cuenta' }, { status: 500 });
    }
}
