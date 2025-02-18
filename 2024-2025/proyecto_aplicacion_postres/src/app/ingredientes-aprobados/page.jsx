"use client";
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import HeaderConSesionP from "/src/app/componentes/HeaderConSesionP";
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faPlus } from '@fortawesome/free-solid-svg-icons';
import "/src/app/style-header.css";
import "/src/app/style-en-espera.css";

/*
* Devuelve la página todos los ingredientes que están 
* aprobados actualmente, pudiendo añadir más si se desea,
* o pudiendo eliminar los que se deseen.
* Está preparado para que cada cierto tiempo, se actualice
* por si se han aprobado más ingredientes. 
*/
export default function IngredientesAprobadosPage() {
    const router = useRouter();
    const [nomUsuario, setNomUsuario] = useState('');

    const [ingredientesAprobados, setIngredientesAprobados] = useState([]);
    const [restricciones, setRestricciones] = useState([]);

    const fetchIngredientesAprobados = async () => {
        try {
            const response = await fetch('/routes/ingredientes?estado=aprobado', { method: 'GET' });
            if (!response.ok) throw new Error("Error al obtener los ingredientes aprobados");
            const data = await response.json();
            setIngredientesAprobados(data);

            // Ahora cargamos las restricciones para cada ingrediente
            data.forEach((ingred) => {
                fetchRestriccionesPorIngrediente(ingred.nom_ingred_ut, ingred.nom_usuario_i_ut);
            });
        } catch (error) {
            console.error("Error al obtener los ingredientes aprobados:", error);
        }
    };

    useEffect(() => {
        const storedNomUsuario = localStorage.getItem('nom_usuario');
        if (storedNomUsuario) {
            setNomUsuario(storedNomUsuario);
        } else {
            router.push('/');
        }
    }, [router]);

    // Cargar ingredientes aprobados al montar el componente
    useEffect(() => {
        fetchIngredientesAprobados();
    }, []);

    useEffect(() => {
        // Llamar a la función al cargar la página
        fetchIngredientesAprobados();
    
        // Configurar un intervalo para actualizar cada 10 segundos
        const intervalId = setInterval(fetchIngredientesAprobados, 10000);
    
        // Limpiar el intervalo cuando el componente se desmonte
        return () => clearInterval(intervalId);
    }, []);

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


    // Función para eliminar un ingrediente
    const handleEliminarIngrediente = async (nom_usuario_i, nom_ingred) => {
        try {
            const response = await fetch('/routes/ingredientes', {
                method: 'DELETE',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ nom_usuario_i, nom_ingred })
            });
            if (response.ok) {
                alert("Ingrediente eliminado correctamente");
                // Actualizar la lista tras eliminar
                fetchIngredientesAprobados(); 
            } else {
                console.error("Error al eliminar el ingrediente");
            }
        } catch (error) {
            console.error("Error al eliminar ingrediente:", error);
        }
    };

    const fetchRestriccionesPorIngrediente = async (nom_ingred, nom_usuario) => {
        try {
            const url = `/routes/tiene_restriccion?nom_ingred=${nom_ingred}&nom_usuario=${nom_usuario}`;
            const response = await fetch(url);
            if (!response.ok) return null;
            const restrictionData = await response.json();
            if (restrictionData !== null) {
                key = `${nom_ingred}-${nom_usuario}`;
                setRestricciones((prevRestricciones) => {
                    const recetaRestricciones = prevRestricciones[key] || [];
                    // Verificamos si la restricción ya está en la lista
                    const exists = recetaRestricciones.some(
                        (restriction) => restriction.nom_restr_t === restrictionData.nom_restr_t
                    );
                    if (!exists) {
                        return {
                            ...prevRestricciones,
                            [key]: [...recetaRestricciones, restrictionData],
                        };
                    }
                    return prevRestricciones;
                })
            }
            return restrictionData;
        } catch (error) {
            console.error("Error al obtener las restricciones del ingrediente:", error);
            return null;
        }
    };

    return (
        <div>
            <HeaderConSesionP
                handleLogout={handleLogout}
                handleVerMisRecetas={handleVerMisRecetas}
                handleVerIngredientesEnEspera={handleVerIngredientesEnEspera}
                handleVerIngredientes={handleVerIngredientes}
                handleAñadirReceta={handleAñadirReceta}
                handleVerRecetasEnEspera={handleVerRecetasEnEspera}
                handleCrearAyudaCabecera={handleCrearAyudaCabecera}
            />

            <div className="en-espera-container">
                <h2>Crear un nuevo ingrediente</h2>

                <div className="boton-crear-container">
                    <button 
                        className="btn-crear-ingrediente"
                        onClick={() => router.push('/anadir-ingrediente/propietario')}
                    >
                        <FontAwesomeIcon icon={faPlus} /> Crear Ingrediente
                    </button>
                </div>

                <h2>Listado de ingredientes aprobados</h2>

                <div className="lista-ingredientes-aprobados">
                    {ingredientesAprobados.length > 0 ? (
                        ingredientesAprobados.map((ingrediente) => (
                            <div key={`${ingrediente.nom_usuario_i}-${ingrediente.nom_ingred}`} className="ingrediente-item">
                                <h3 className="titulo-ingrediente">{ingrediente.nom_ingred}</h3>
                                <p><strong>Creado por:</strong> {ingrediente.nom_usuario_i}</p>
                                {/* <p><strong>Restricciones:</strong> {ingrediente.tiene_Restr.length > 0 
                                    ? ingrediente.tiene_Restr.map(restr => restr.restriccion.nom_restr).join(', ')
                                    : 'Sin restricciones'}
                                </p> */}
                                <p>Restricciones:</p>
                                {restricciones[`${ingrediente.nom_usuario_i}-${ingrediente.nom_ingred}`] && restricciones[`${ingrediente.nom_usuario_i}-${ingrediente.nom_ingred}`].map((restr) => (
                                    <p key={restr.nom_restr_t}>
                                        {restr.nom_restr_t}
                                    </p>
                                ))}
                                <button
                                    onClick={() => handleEliminarIngrediente(ingrediente.nom_usuario_i, ingrediente.nom_ingred)}
                                    className="btn-borrar"
                                >
                                    Borrar
                                </button>
                            </div>
                        ))
                    ) : (
                        <p className="no-elementos">No hay ingredientes aprobados</p>
                    )}
                </div>
            </div>
        </div>
    );
}
