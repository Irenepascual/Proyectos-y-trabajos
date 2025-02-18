"use client";
import "/src/app/style-header.css";
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import HeaderConSesionP from "/src/app/componentes/HeaderConSesionP";
import RecetasListSinFav from "/src/app/componentes/BuscadorListadoRecetasSinFav.js";

/*
* Devuelve la página de home del propietario,
* donde se muestra la cabecera del propietario y
* todas las recetas disponibles. 
* El nombre del propietario lo obtiene del 
* almacenamiento local. 
*/
function HomeConSesionP() {
    const router = useRouter();
    const [nomUsuario, setNomUsuario] = useState('');  // Estado para el nombre de usuario

    // Obtener el nombre de usuario al cargar la página
    useEffect(() => {
        const storedNomUsuario = localStorage.getItem('nom_usuario');
        if (storedNomUsuario) {
            setNomUsuario(storedNomUsuario);
        } else {
            router.push('/');
        }
    }, [router]);

    // Función para manejar el cierre de sesión
    const handleLogout = () => {
        localStorage.removeItem('nom_usuario');
        setNomUsuario('');
        router.push('/');
    };

    // Funciones para cambiar entre vistas
    const handleVerRecetasEnEspera = () => router.push('/en-espera-recetas');
    const handleVerIngredientesEnEspera = () => router.push('/en-espera-ingredientes');
    const handleVerMisRecetas = () => router.push('/mis-recetas/propietario');
    const handleAñadirReceta = () => router.push('/anadir-receta/propietario');
    const handleAñadirIngrediente = () => router.push('/anadir-ingrediente/propietario');
    const handleVerIngredientes = () => router.push('/ingredientes-aprobados');
    const handleCrearAyudaCabecera = () => router.push('/crearAyuda');
    const handleResponder = () => router.push('/peticiones');
    
    return (
        <div className="ancho-completo">
            <HeaderConSesionP
                handleLogout={handleLogout}
                handleVerMisRecetas={handleVerMisRecetas}
                handleVerIngredientesEnEspera={handleVerIngredientesEnEspera}
                handleVerIngredientes={handleVerIngredientes}
                handleAñadirReceta={handleAñadirReceta}
                handleVerRecetasEnEspera={handleVerRecetasEnEspera}
                handleCrearAyudaCabecera={handleCrearAyudaCabecera}
                handleResponder={handleResponder}
            />
            
            <RecetasListSinFav />
        </div>
    );
}

export default HomeConSesionP;
