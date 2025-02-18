"use client";
import "/src/app/style-header.css";
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import HeaderConSesionP from "/src/app/componentes/HeaderConSesionP";
import AnadirReceta from "/src/app/anadir-receta/propietario/AnadirReceta.js";

/*
* Devuelve la página de añadir receta, donde se muestra
* la cabecera de propietario y permite añadir una receta
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

    const handleVerMisRecetas = () => router.push('/mis-recetas/propietario');
    const handleVerRecetasEnEspera = () => router.push('/en-espera-recetas');
    const handleVerIngredientesEnEspera = () => router.push('/en-espera-ingredientes');
    const handleVerIngredientes = () => router.push('/ingredientes-aprobados');
    const handleAñadirReceta = () => router.push('/anadir-receta/propietario');
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
            <AnadirReceta nomUsuario={nomUsuario} />
        </div>
    );
}

export default HomeConSesion;
