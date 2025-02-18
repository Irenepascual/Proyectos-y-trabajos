import { NextResponse } from 'next/server';
import db from '@/libs/db'; // Conexión a la base de datos

/*
* Devuelve todos los favoritos de un usuario
*/
export async function GET(req) {
    const { searchParams } = new URL(req.url);
    const usuario = searchParams.get('usuario'); // Obtener el nombre del usuario desde los parámetros de búsqueda

    try {
        // Buscar los favoritos del usuario en la base de datos

        const favoritos = [];
        let skip = 0; 

        while(true){
            const favorito = await db.favoritos.findFirst({
                where: { nom_usuario_u_f: usuario },
                skip,
                select: { nom_receta_f: true } // Obtener solo los nombres de las recetas
            });
            if (!favorito) break;
            favoritos.push(favorito);
            skip++;
        }
        const recetaFavoritos = favoritos.map(fav => fav.nom_receta_f);
        return NextResponse.json({ favoritos: recetaFavoritos });
    } catch (error) {
        console.error('Error al obtener los favoritos', error);
        return NextResponse.json({ error: 'Error al obtener los favoritos' }, { status: 500 });
    }
}