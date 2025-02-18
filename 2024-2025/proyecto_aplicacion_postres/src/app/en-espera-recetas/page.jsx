"use client";
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import HeaderConSesionP from "/src/app/componentes/HeaderConSesionP";
import "/src/app/style-header.css";
import "/src/app/style-en-espera.css";

/*
* Devuelve la página de en espera recetas para 
* propietario, donde se muestra la cabecera del propietario, 
* todas las recetas que no están aprobadas todavía (con su respectiva
* información) y dos botones para poder aceptar la receta (aprobandolo)
* o para poder eliminar la receta (eliminándolo de la base de datos).
*/
export default function EnEsperaPage() {
    const [recetasEnEspera, setRecetasEnEspera] = useState([]);
    const [recetasRechazadas, setRecetasRechazadas] = useState([]);
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

    // useEffect(() => {

    //     fetchRecetasEnEspera();
    //     fetchRecetasRechazadas();
    
    //     // Configurar un intervalo para actualizar cada 10 segundos
    //     const intervalId = setInterval(fetchRecetasEnEspera, 10000);
    
    //     // Limpiar el intervalo cuando el componente se desmonte
    //     return () => clearInterval(intervalId);
    // }, []);
    useEffect(() => {
        // Función que ejecutará ambas actualizaciones
        const updateRecetas = () => {
            fetchRecetasEnEspera();
            fetchRecetasRechazadas();
        };
        updateRecetas();
        const intervalId = setInterval(updateRecetas, 10000);
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

    // Obtener todas las recetas en espera (estado = "no_aprobado")
    const fetchRecetasEnEspera = async () => {
        try {
            const response = await fetch('/routes/recetas?estado=no_aprobado', { method: 'GET' });
            if (!response.ok) throw new Error("Error al obtener las recetas en espera");
            const data = await response.json();
            setRecetasEnEspera(data);
        } catch (error) {
            console.error("Error al obtener las recetas en espera:", error);
        }
    };

    // Obtener todas las recetas rechazadas (estado = "rechazado"")
    const fetchRecetasRechazadas = async () => {
        try {
            const response = await fetch('/routes/recetas?estado=rechazado', { method: 'GET' });
            if (!response.ok) throw new Error("Error al obtener las recetas rechazadas");
            const data = await response.json();
            setRecetasRechazadas(data);
        } catch (error) {
            console.error("Error al obtener las recetas rechazadas:", error);
        }
    };

    useEffect(() => {
        fetchRecetasEnEspera();
        fetchRecetasRechazadas();
    }, []);

    // Función para eliminar una receta en espera
    const handleEliminarReceta = async (nom_usuario_r, nom_receta, fecha_creac_receta) => {
        try {
            const response = await fetch('/routes/recetas', {
                method: 'DELETE',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ nom_usuario_r, nom_receta, fecha_creac_receta }),
            });
            if (response.ok) {
                alert("Receta eliminada correctamente");
                fetchRecetasEnEspera(); // Recargar la lista de recetas
            }
        } catch (error) {
            console.error("Error al eliminar receta:", error);
        }
    };

    // Función para eliminar una receta en espera
    const handleRechazarReceta = async (nom_usuario_r, nom_receta, fecha_creac_receta) => {
        try {
            const estado = "rechazado";
            const response = await fetch('/routes/recetas', {
                method: 'PUT',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ nom_usuario_r, nom_receta, fecha_creac_receta, estado }),
            });
            if (response.ok) {
                alert("Receta rechazada correctamente");
                fetchRecetasEnEspera(); // Recargar la lista de recetas
            }
        } catch (error) {
            console.error("Error al rechazar receta:", error);
        }
    };

    // Función para aceptar una receta
    const handleAprobarReceta = async (nom_usuario_r, nom_receta, fecha_creac_receta) => {
        try {
            const estado = "aprobado";
            const response = await fetch('/routes/recetas', {
                method: 'PUT',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ nom_usuario_r, nom_receta, fecha_creac_receta, estado }),
            });
            if (response.ok) {
                alert("Receta aprobada correctamente");
                fetchRecetasEnEspera(); // Recargar la lista de recetas
            }
        } catch (error) {
            console.error("Error al aprobar receta:", error);
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

            <p className="titulo-en-espera">Listado de recetas no aprobadas</p>

            <div className="lista-recetas-en-espera">
                {recetasEnEspera.length > 0 ? (
                    recetasEnEspera.map((receta) => (
                        <div 
                            key={`${receta.nom_usuario_r}-${receta.nom_receta}-${receta.fecha_creac_receta}`} 
                            className="receta-espera-item"
                        >
                            <h3 className="titulo-receta-espera">{receta.nom_receta}</h3>
                            <p><strong>Creado por:</strong> {receta.nom_usuario_r}</p>
                            <p className="descripcion-receta-espera">{receta.descripcion || 'Sin descripción'}</p>
                            <p className="fecha-receta-espera"><strong>Fecha de creación:</strong> {new Date(receta.fecha_creac_receta).toLocaleDateString()}</p>
                            
                            <div className="botones-acciones">
                                {/* Botón para eliminar la receta */}
                                <button 
                                    onClick={() => handleEliminarReceta(receta.nom_usuario_r, receta.nom_receta, receta.fecha_creac_receta)} 
                                    className="btn-eliminar"
                                >
                                    Eliminar
                                </button>

                                {/* Botón para rechazar la receta */}
                                <button 
                                    onClick={() => handleRechazarReceta(receta.nom_usuario_r, receta.nom_receta, receta.fecha_creac_receta)} 
                                    className="btn-rechazar"
                                >
                                    Rechazar
                                </button>
                                
                                {/* Botón para aceptar la receta */}
                                <button 
                                    onClick={() => handleAprobarReceta(receta.nom_usuario_r, receta.nom_receta, receta.fecha_creac_receta)} 
                                    className="btn-aceptar"
                                >
                                    Aceptar
                                </button>
                            </div>
                        </div>
                    ))
                ) : (
                    <p>No hay recetas en espera</p>
                )}
            </div>

            <p className="titulo-en-espera">Listado de recetas rechazadas</p>

            <div className="lista-recetas-en-espera">
                {recetasRechazadas.length > 0 ? (
                    recetasRechazadas.map((receta) => (
                        <div 
                            key={`${receta.nom_usuario_r}-${receta.nom_receta}-${receta.fecha_creac_receta}`} 
                            className="receta-espera-item"
                        >
                            <h3 className="titulo-receta-espera">{receta.nom_receta}</h3>
                            <p><strong>Creado por:</strong> {receta.nom_usuario_r}</p>
                            <p className="descripcion-receta-espera">{receta.descripcion || 'Sin descripción'}</p>
                            <p className="fecha-receta-espera"><strong>Fecha de creación:</strong> {new Date(receta.fecha_creac_receta).toLocaleDateString()}</p>
                            
                            <div className="botones-acciones">
                                {/* Botón para eliminar la receta */}
                                <button 
                                    onClick={() => handleEliminarReceta(receta.nom_usuario_r, receta.nom_receta, receta.fecha_creac_receta)} 
                                    className="btn-eliminar"
                                >
                                    Eliminar
                                </button>
                                
                                {/* Botón para aceptar la receta */}
                                <button 
                                    onClick={() => handleAprobarReceta(receta.nom_usuario_r, receta.nom_receta, receta.fecha_creac_receta)} 
                                    className="btn-aceptar"
                                >
                                    Aceptar
                                </button>
                            </div>
                        </div>
                    ))
                ) : (
                    <p>No hay recetas rechazadas</p>
                )}
            </div>

        </div>
    );
}
