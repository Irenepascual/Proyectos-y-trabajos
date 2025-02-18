"use client";
import "/src/app/style-header.css";
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import HeaderConSesionP from "/src/app/componentes/HeaderConSesionP";
import MisRecetas from "/src/app/mis-recetas/propietario/MisRecetas.js";

/*
* Devuelve la página de mis recetas, donde se muestra
* la cabecera de propietario y permite ver todas
* las recetas creadas por ese propietario
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
            
            <MisRecetas nomUsuario={nomUsuario} />
        </div>
    );
}

export default MisRecetasPage;
