import { NextResponse } from "next/server";
import db from '@/libs/db';

/*
* Registra un propietario en la base de datos, mientras
* no coincida el nombre del propietario, el correo o el
* código con alguno ya existente; o mientras el código
* sea un número válido. El propietario se guardará en la 
* tabla de propietario y usuario.
*/
export async function POST(request) {
    try {
        const data = await request.json();

        // Verificación de campos obligatorios
        if (!data.nom_usuario || !data.correo || !data.contrasena || !data.codigo) {
            console.log("Faltan datos obligatorios:", data);
            return NextResponse.json({ message: 'Faltan datos obligatorios' }, { status: 400 });
        }

        console.log("Datos recibidos para crear propietario:", data);

        // Convertir 'codigo' a un número entero
        const codigo = parseInt(data.codigo, 10);
        if (isNaN(codigo)) {
            return NextResponse.json(
                { message: "El código debe ser un número válido" },
                { status: 400 }
            );
        }

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

        // Verificar si ya existe un propietario con el mismo código
        const codigoExists = await db.propietario.findFirst({
            where: { codigo }
        });
        if (codigoExists) {
            console.log("El código ya está en uso:", codigo);
            return NextResponse.json(
                { message: "El código ya está en uso" },
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

        // Crear el propietario
        const newOwner = await db.propietario.create({
            data: {
                nom_usuario_p: newUser.nom_usuario,
                codigo,
            },
        });

        console.log("Propietario creado exitosamente:", newOwner);

        return NextResponse.json({ message: 'Propietario creado exitosamente', owner: newOwner });

    } catch (error) {
        console.error("Error en el proceso de guardado:", error);
        return NextResponse.json({ message: 'Error al procesar la solicitud' }, { status: 500 });
    }
}
