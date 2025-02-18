"use client";
import "/src/app/style-header.css";
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import HeaderConSesion from "/src/app/componentes/HeaderConSesion";
import MisFavoritos from "/src/app/mis-favoritos/MisFavoritos.js";

// Crea una pagina que muestra los Favoritos de un usuario haciendo la gestion de la redireccion 
// basada en el estado del nombre de usuario almacenado en el localStorage. 

function MisFavoritosPage() {
    const router = useRouter();
    // Estado para el nombre de usuario
    const [nomUsuario, setNomUsuario] = useState('');  

    // Obtener el nombre de usuario del localStorage
    // Actualizar el estado con el nombre de usuario del localStorage
    // Este efecto se ejecuta solo una vez cuando se monta el componente
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
            
            <MisFavoritos nomUsuario={nomUsuario} />
        </div>
    );
}

export default MisFavoritosPage;
