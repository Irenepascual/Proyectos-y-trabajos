import { NextResponse } from "next/server";
import db from '@/libs/db';

/*
* Registra un usuario en la base de datos, mientras
* no coincida el nombre de usuario o el correo con 
* alguno ya existente. El usuario se guardará en la 
* tabla de usuario_registrado y usuario.
*/
export async function POST(request) {
    try {
        const data = await request.json();

        // Verificación de campos obligatorios
        if (!data.nom_usuario || !data.correo || !data.contrasena) {
            console.log("Faltan datos obligatorios:", data);
            return NextResponse.json({ message: 'Faltan datos obligatorios' }, { status: 400 });
        }

        console.log("Datos recibidos para crear usuario registrado:", data);

        // Verificar si ya existe un usuario con el mismo correo
        const emailExists = await db.usuario.findFirst({
            where: { correo: data.correo }
        });

        if (emailExists) {
            console.log("El correo ya está en uso:", data.correo);
            return NextResponse.json(
                { message: "El correo ya está en uso" },
                { status: 400 }
            );
        }

        // Verificar si ya existe un usuario con el mismo nombre de usuario
        const userExists = await db.usuario.findFirst({
            where: { nom_usuario: data.nom_usuario }
        });

        if (userExists) {
            return NextResponse.json(
                { message: "El nombre de usuario ya existe" },
                { status: 400 }
            );
        }

        // Crear el nuevo usuario
        const newUser = await db.usuario.create({
            data: {
                nom_usuario: data.nom_usuario,
                correo: data.correo,
                contrasena: data.contrasena,
            },
        });

        console.log("Usuario creado exitosamente:", newUser);

        // Crear el registro en Usuario_registrado
        const newRegistratedUser = await db.usuario_registrado.create({
            data: {
                nom_usuario_u: newUser.nom_usuario,
            },
        });

        console.log("Usuario registrado creado exitosamente:", newRegistratedUser);

        return NextResponse.json({ message: 'Usuario registrado creado exitosamente', user: newRegistratedUser });

    } catch (error) {
        console.error("Error en el proceso de guardado:", error);
        return NextResponse.json({ message: 'Error al procesar la solicitud' }, { status: 500 });
    }
}
