"use client";
import "/src/app/style-header.css";
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import HeaderConSesion from "/src/app/componentes/HeaderConSesion";
import MisRecetas from "/src/app/mis-recetas/usuario/MisRecetas.js";

/*
* Devuelve la página de mis recetas, donde se muestra
* la cabecera de usuario y permite ver todas
* las recetas creadas por ese usuario
*/
function MisRecetasPage() {
    const router = useRouter();
    const [nomUsuario, setNomUsuario] = useState('');  // Estado para el nombre de usuario

    // Obtener el nombre de usuario del localStorage
    useEffect(() => {
        const storedNomUsuario = localStorage.getItem('nom_usuario');
        if (storedNomUsuario) {
            setNomUsuario(storedNomUsuario); // Actualizar el estado con el nombre de usuario del localStorage
        } else {
            router.push('/');
        }
    }, [router]);  // Este efecto se ejecuta solo una vez cuando se monta el componente


    const handleLogout = () => {
        localStorage.removeItem('nom_usuario');
        setNomUsuario('');
        router.push('/');
    };

    const handleContacto = () => router.push('/contacto/usuario');
    const handleAyuda = () => router.push('/ayuda/usuario');
    const handleVerMisRecetas = () => router.push('/mis-recetas/usuario');
    const handleVerMisFavoritos = () => router.push('/mis-favoritos');
    const handleAñadirReceta = () => router.push('/anadir-receta/usuario');
    const handleAñadirIngrediente = () => router.push('/anadir-ingrediente/usuario')

    return (
        <div className="ancho-completo">
            <HeaderConSesion
                handleLogout={handleLogout}
                handleVerMisFavoritos={handleVerMisFavoritos}
                handleVerMisRecetas={handleVerMisRecetas}
                handleAñadirReceta={handleAñadirReceta}
                handleAñadirIngrediente={handleAñadirIngrediente}
                handleContacto={handleContacto}
                handleAyuda={handleAyuda}
            />
            
            <MisRecetas nomUsuario={nomUsuario} />
        </div>
    );
}

export default MisRecetasPage;
