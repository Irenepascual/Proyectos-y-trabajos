"use client";
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import HeaderConSesionP from "/src/app/componentes/HeaderConSesionP";
import "/src/app/style-header.css";
import "/src/app/style-en-espera.css";

/*
* Devuelve la página de en espera ingredientes para 
* propietario, donde se muestra la cabecera del propietario, 
* todos los ingredientes que no están aprobados todavía y dos 
* botones para poder aceptar el ingrediente (aprobandolo) o para
* poder eliminar el ingrediente (eliminándolo de la base de datos).
*/
export default function EnEsperaIngredientesPage() {
    const router = useRouter();
    const [nomUsuario, setNomUsuario] = useState('');

    const [ingredientesEnEspera, setIngredientesEnEspera] = useState([]);
    const [ingredientesRechazados, setIngredientesRechazados] = useState([]);
    const [restricciones, setRestricciones] = useState([]);

    useEffect(() => {
        const storedNomUsuario = localStorage.getItem('nom_usuario');
        if (storedNomUsuario) {
            setNomUsuario(storedNomUsuario);
        } else {
            router.push('/');
        }
    }, [router]);

    useEffect(() => {
        // Función que ejecutará ambas actualizaciones
        const updateIngredientes = () => {
            fetchIngredientesEnEspera();
            fetchIngredientesRechazados();
        };
        updateIngredientes();    
        const intervalId = setInterval(updateIngredientes, 10000);
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
    const handleResponder = () => router.push('/peticiones');

    // Obtener todos los ingredientes en espera (aprobado = false)
    const fetchIngredientesEnEspera = async () => {
        try {
            const response = await fetch('/routes/ingredientes?estado=no_aprobado', { method: 'GET' });
            if (!response.ok) throw new Error("Error al obtener los ingredientes en espera");
            const data = await response.json();
            setIngredientesEnEspera(data);

            // // Ahora cargamos las restricciones para cada ingrediente
            data.forEach((ingred) => {
                fetchRestriccionesPorIngrediente(ingred.nom_ingred, ingred.nom_usuario_i);
            });
            // Cargar las restricciones de cada ingrediente
            // await Promise.all(data.map(ingred => fetchRestriccionesPorIngrediente(ingred.nom_ingred, ingred.nom_usuario_i)));
        } catch (error) {
            console.error("Error al obtener los ingredientes en espera:", error);
        }
    };

    // Obtener todos los ingredientes en espera (aprobado = false)
    const fetchIngredientesRechazados = async () => {
        try {
            const response = await fetch('/routes/ingredientes?estado=rechazado', { method: 'GET' });
            if (!response.ok) throw new Error("Error al obtener los ingredientes rechazados");
            const data = await response.json();
            setIngredientesRechazados(data);

            // // Ahora cargamos las restricciones para cada ingrediente
            data.forEach((ingred) => {
                fetchRestriccionesPorIngrediente(ingred.nom_ingred, ingred.nom_usuario_i);
            });
            // Cargar las restricciones de cada ingrediente
            // await Promise.all(data.map(ingred => fetchRestriccionesPorIngrediente(ingred.nom_ingred, ingred.nom_usuario_i)));
        } catch (error) {
            console.error("Error al obtener los ingredientes rechazados:", error);
        }
    };

    const fetchRestriccionesPorIngrediente = async (nom_ingred, nom_usuario) => {
        try {
            const url = `/routes/tiene_restriccion?nom_ingred=${nom_ingred}&nom_usuario=${nom_usuario}`;
            const response = await fetch(url);
            if (!response.ok) return null;
            const restrictionData = await response.json();
            if (restrictionData !== null) {
                console.log("RESTRICCIONES:", restrictionData);
                const key = `${nom_ingred}-${nom_usuario}`;
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

    useEffect(() => {
        fetchIngredientesEnEspera();
        fetchIngredientesRechazados();
    }, []);

    // Función para eliminar un ingrediente en espera
    const handleEliminarIngrediente = async (nom_usuario_i, nom_ingred) => {
        try {
            const response = await fetch('/routes/ingredientes', {
                method: 'DELETE',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ nom_usuario_i, nom_ingred }),
            });
            if (response.ok) {
                alert("Ingrediente eliminado correctamente");
                fetchIngredientesEnEspera(); // Recargar la lista de ingredientes
            }
        } catch (error) {
            console.error("Error al eliminar ingrediente:", error);
        }
    };

    // Función para aceptar un ingrediente
    const handleAceptarIngrediente = async (nom_usuario_i, nom_ingred) => {
        try {
            const estado = "aprobado";
            const response = await fetch('/routes/ingredientes', {
                method: 'PUT',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ nom_usuario_i, nom_ingred, estado }),
            });
            if (response.ok) {
                alert("Ingrediente aprobado correctamente");
                fetchIngredientesEnEspera(); // Recargar la lista de ingredientes
            }
        } catch (error) {
            console.error("Error al aprobar ingrediente:", error);
        }
    };

    // Función para rechazar un ingrediente
    const handleRechazarIngrediente = async (nom_usuario_i, nom_ingred) => {
        try {
            const estado = "rechazado";
            const response = await fetch('/routes/ingredientes', {
                method: 'PUT',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ nom_usuario_i, nom_ingred, estado }),
            });
            if (response.ok) {
                alert("Ingrediente rechazado correctamente");
                fetchIngredientesEnEspera(); // Recargar la lista de ingredientes
            }
        } catch (error) {
            console.error("Error al rechazar el ingrediente:", error);
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
                handleResponder={handleResponder}
            />

            <p className="titulo-en-espera">Listado de ingredientes no aprobados</p>

            <div className="lista-ingredientes-en-espera">
                {ingredientesEnEspera.length > 0 ? (
                    ingredientesEnEspera.map((ingrediente) => (
                        <div key={`${ingrediente.nom_usuario_i}-${ingrediente.nom_ingred}`} className="ingrediente-espera-item">
                            <h3 className="titulo-ingrediente-espera">{ingrediente.nom_ingred}</h3>
                            <p><strong>Creado por:</strong> {ingrediente.nom_usuario_i}</p>
                            
                            <p>Restricciones:</p>
                            {restricciones[`${ingrediente.nom_ingred}-${ingrediente.nom_usuario_i}`] &&
                            restricciones[`${ingrediente.nom_ingred}-${ingrediente.nom_usuario_i}`].length > 0 ? (
                                restricciones[`${ingrediente.nom_ingred}-${ingrediente.nom_usuario_i}`].map((restr) => (
                                    <p key={restr.nom_restr_t}>
                                        {restr.nom_restr_t}
                                    </p>
                                ))
                            ) : (
                                <p>No hay</p>
                            )}


                            <div className="botones-acciones">
                                <button
                                    onClick={() => handleEliminarIngrediente(ingrediente.nom_usuario_i, ingrediente.nom_ingred)}
                                    className="btn-eliminar"
                                >
                                    Eliminar
                                </button>

                                <button
                                    onClick={() => handleRechazarIngrediente(ingrediente.nom_usuario_i, ingrediente.nom_ingred)}
                                    className="btn-rechazar"
                                >
                                    Rechazar
                                </button>

                                <button
                                    onClick={() => handleAceptarIngrediente(ingrediente.nom_usuario_i, ingrediente.nom_ingred)}
                                    className="btn-aceptar"
                                >
                                    Aceptar
                                </button>
                            </div>
                        </div>
                    ))
                ) : (
                    <p>No hay ingredientes en espera</p>
                )}
            </div>


            <p className="titulo-en-espera">Listado de ingredientes rechazados</p>

            <div className="lista-ingredientes-en-espera">
                {ingredientesRechazados.length > 0 ? (
                    ingredientesRechazados.map((ingrediente) => (
                        <div key={`${ingrediente.nom_usuario_i}-${ingrediente.nom_ingred}`} className="ingrediente-espera-item">
                            <h3 className="titulo-ingrediente-espera">{ingrediente.nom_ingred}</h3>
                            <p><strong>Creado por:</strong> {ingrediente.nom_usuario_i}</p>

                            <p>Restricciones:</p>
                            {restricciones[`${ingrediente.nom_ingred}-${ingrediente.nom_usuario_i}`] &&
                            restricciones[`${ingrediente.nom_ingred}-${ingrediente.nom_usuario_i}`].length > 0 ? (
                                restricciones[`${ingrediente.nom_ingred}-${ingrediente.nom_usuario_i}`].map((restr) => (
                                    <p key={restr.nom_restr_t}>
                                        {restr.nom_restr_t}
                                    </p>
                                ))
                            ) : (
                                <p>No hay</p>
                            )}

                            <div className="botones-acciones">
                                <button
                                    onClick={() => handleEliminarIngrediente(ingrediente.nom_usuario_i, ingrediente.nom_ingred)}
                                    className="btn-eliminar"
                                >
                                    Eliminar
                                </button>

                                <button
                                    onClick={() => handleAceptarIngrediente(ingrediente.nom_usuario_i, ingrediente.nom_ingred)}
                                    className="btn-aceptar"
                                >
                                    Aceptar
                                </button>
                            </div>
                        </div>
                    ))
                ) : (
                    <p>No hay ingredientes en espera</p>
                )}
            </div>
        </div>
    );
}
