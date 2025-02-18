import { NextResponse } from "next/server";
import db from '@/libs/db';

/*
* Cuando se intenta iniciar sesión como usuario,
* se comprueba que exista el usuario que pretende
* iniciar sesión en la tabla de usuario registrado, 
* y verifica que esa sea su contraseña. 
*/
export async function POST(request) {
    try {
        const data = await request.json();
        
        // Buscar el usuario registrado
        const userFound = await db.usuario_registrado.findFirst({
            where: {
                nom_usuario_u: data.nom_usuario
            }
        })
        if (!userFound) {
            throw new Error('Usuario registrado no encontrado');
        }

        // Verificar la contraseña
        const matchPassword = await db.usuario.findFirst({
            where: {
                nom_usuario: data.nom_usuario,
                contrasena: data.contrasena,
            }
        });
        if (!matchPassword) {
            throw new Error('Contraseña incorrecta');
        }
            
        return NextResponse.json(true);

    } catch (error) {
        return NextResponse.json(
            { message: error.message },
            { status: 500 }
        );
    }
}
