"use client";
import "/src/app/style-header.css";
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import HeaderConSesion from "/src/app/componentes/HeaderConSesion";
import AnadirReceta from "/src/app/anadir-receta/usuario/AnadirReceta.js";

/*
* Devuelve la página de añadir receta, donde se muestra
* la cabecera de usuario registrado y permite añadir una receta
*/
function HomeConSesion() {
    const router = useRouter();
    const [nomUsuario, setNomUsuario] = useState('');  

    useEffect(() => {
        const storedNomUsuario = localStorage.getItem('nom_usuario');
        if (storedNomUsuario) {
            setNomUsuario(storedNomUsuario); 
        } else {
            router.push('/');
        }
    }, [router]);  


    const handleLogout = () => {
        localStorage.removeItem('nom_usuario');
        setNomUsuario('');
        router.push('/');
    };

    const handleContacto = () => router.push('/contacto/usuario');
    const handleAyuda = () => router.push('/ayuda/usuario/usuario');
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
            <AnadirReceta nomUsuario={nomUsuario} />
        </div>
    );
}

export default HomeConSesion;
