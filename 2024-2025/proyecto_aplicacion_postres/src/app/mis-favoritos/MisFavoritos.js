'use client';
import React, { useEffect, useState } from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faPeopleGroup, faClock } from '@fortawesome/free-solid-svg-icons'; // Asegúrate de que estos iconos estén importados
import "/src/app/style-recetas.css";
import "/src/app/style-recetaUnica.css";

export default function MisFavoritos({ nomUsuario }) {
    const [loading, setLoading] = useState(true);

    const [favoritos, setFavoritos] = useState([]);

    const [recetas, setRecetas] = useState([]);
    const [selectedReceta, setSelectedReceta] = useState(null);
    const [selectedRecetaKey, setSelectedRecetaKey] = useState(null);

    const [utiliza, setUtiliza] = useState([]);
    const [restricciones, setRestricciones] = useState([]);

    const handleVerMasReceta = (receta) => {
        // Establecemos la receta seleccionada
        const recetaKey = `${receta.nom_usuario_r}-${receta.nom_receta}-${receta.fecha_creac_receta}`;
        setSelectedReceta(receta);
        setSelectedRecetaKey(recetaKey);
    };

    const handleCloseDetails = () => {
        setSelectedReceta(null);
    };

    const fetchFavoritos = async (query) => {
        try {
            const url = `/routes/favoritos?nom_usuario=${query}`;
            const response = await fetch(url);
            if (!response.ok) throw new Error(response.error);
            const data = await response.json();
            setFavoritos(data);
        } catch (error) {
            console.error("Error al obtener los favoritos:", error);
        } finally {
            setLoading(false);
        }
    };

    const fetchRecetaByFavorito = async (nom_receta, nom_usuario, fecha_creac_receta) => {
        try {
            const url = `/routes/recetas?nom_receta=${nom_receta}&nom_usuario=${nom_usuario}&fecha_creac_receta=${fecha_creac_receta}`;
            const response = await fetch(url);
            if (!response.ok) throw new Error("Error al obtener la receta");
            const recetaData = await response.json();
            setRecetas((prevRecetas) => ({
                ...prevRecetas,
                [`${nom_receta}-${nom_usuario}-${fecha_creac_receta}`]: recetaData,
            }));
        } catch (error) {
            console.error("Error al obtener la receta:", error);
        }
    };

    const fetchUtiliza = async (nom_usuario_r, nom_receta, fecha_creac_receta) => {
        try {
            const url = `/routes/utilizaIngrediente?nom_usuario_r=${nom_usuario_r}&nom_receta=${nom_receta}&fecha_creac_receta=${fecha_creac_receta}`;
            const response = await fetch(url);
            if (!response.ok) throw new Error("Error al obtener los ingredientes de la receta");
            const data = await response.json();
            // Actualizamos los ingredientes de la receta
            const key = `${nom_usuario_r}-${nom_receta}-${fecha_creac_receta}`;
            setUtiliza((ut) => ({
                ...ut,
                [key]: data,
            }));
            // Ahora cargamos las restricciones para cada ingrediente de esta receta
            data.forEach((ingred) => {
                fetchRestriccionesPorIngrediente(ingred.nom_ingred_ut, ingred.nom_usuario_i_ut, key);
            });
        } catch (error) {
            console.error("Error al obtener los ingredientes de la receta:", error);
        }
    };

    const fetchRestriccionesPorIngrediente = async (nom_ingred, nom_usuario, recetaKey) => {
        try {
            const url = `/routes/tiene_restriccion?nom_ingred=${nom_ingred}&nom_usuario=${nom_usuario}`;
            const response = await fetch(url);
            if (!response.ok) return null;
            const restrictionData = await response.json();
            if (restrictionData !== null) {
                // Almacenamos las restricciones de esta receta
                setRestricciones((prevRestricciones) => {
                    const key = recetaKey; // Usamos la clave de la receta para almacenarlas
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
                });
            }
            return restrictionData;
        } catch (error) {
            console.error("Error al obtener las restricciones de la receta:", error);
            return null;
        }
    };

    useEffect(() => {
        if (nomUsuario) fetchFavoritos(nomUsuario);
    }, [nomUsuario]);

    useEffect(() => {
        favoritos.forEach((favorito) => {
            fetchRecetaByFavorito(favorito.nom_receta_f, favorito.nom_usuario_r_f, favorito.fecha_creac_receta_f);
            fetchUtiliza(favorito.nom_usuario_r_f, favorito.nom_receta_f, favorito.fecha_creac_receta_f);
        });
    }, [favoritos]);

    return (
        <div className="grid-container">
            {loading ? (
                <div className="loading-container">
                    <div className="spinner"></div>
                </div>
            ) : selectedReceta ? (
                <div>
                    <button onClick={handleCloseDetails} className="u-boton-volver">Volver</button>
                    <p className="u-nom-receta">{selectedReceta.nom_receta}</p>
                    <div className="u-img-otros-container">
                        <div>
                            <img className="u-imagen" src={selectedReceta.foto} alt={selectedReceta.nom_receta} />
                            <div className="u-texto-creacion">
                                <div className="u-misma-linea">
                                    <p className="u-tit-texto-creacion">Creador:</p>
                                    <p> {selectedReceta.nom_usuario_r}</p>
                                </div>
                                <div className="u-misma-linea">
                                    <p className="u-tit-texto-creacion">Fecha de creación:</p>
                                    <p> {new Date(selectedReceta.fecha_creac_receta).toLocaleString("en-US", {
                                            day: "numeric",
                                            month: "numeric",
                                            year: "numeric",
                                            hour: "numeric",
                                            minute: "numeric",
                                            second: "numeric",
                                            hour12: true,
                                        })}
                                    </p>
                                </div>
                            </div>
                        </div>
                        <div className="u-datos-container">
                            <div className="u-div-pers-tiempo">
                                <FontAwesomeIcon icon={faPeopleGroup} className="u-icono-ari"/>
                                <p className="u-texto-pers-tiempo">Número de personas: {selectedReceta.num_personas}</p>
                                <FontAwesomeIcon icon={faClock} className="u-icono-ari"/>
                                <p className="u-texto-pers-tiempo">Tiempo de preparación: {selectedReceta.tiempo_prep}</p>
                            </div>
                            <div className="u-ingred-utiliza-container">
                                <p className="u-tit">Ingredientes necesarios:</p>
                                {utiliza[`${selectedReceta.nom_usuario_r}-${selectedReceta.nom_receta}-${selectedReceta.fecha_creac_receta}`] ? (
                                    <ul className="u-ingred-lista">
                                        {utiliza[`${selectedReceta.nom_usuario_r}-${selectedReceta.nom_receta}-${selectedReceta.fecha_creac_receta}`].map((ingrediente) => (
                                            <li key={`${ingrediente.nom_ingred_ut}-${ingrediente.nom_usuario_i_ut}`}>
                                                {ingrediente.cantidad} {ingrediente.unidad_med} {ingrediente.nom_ingred_ut}
                                            </li>
                                        ))}
                                    </ul>
                                ) : (
                                    <p>No hay ingredientes</p>
                                )}
                            </div>
                            <p className="u-tit">Descripción:</p>
                            <p className="u-descripcion">{selectedReceta.descripcion}</p>
                            <div>
                                <p className="u-tit">Restricciones:</p>
                                {restricciones[selectedRecetaKey] && restricciones[selectedRecetaKey].length > 0 ? (
                                    restricciones[selectedRecetaKey].map((restr) => (
                                        <p key={restr.nom_restr_t}>
                                            {restr.nom_restr_t === "huevo" ? (
                                                <span role="img" aria-label="Egg">🥚</span>
                                            ) : restr.nom_restr_t === "gluten" ? (
                                                <span role="img" aria-label="Gluten">🌾</span>
                                            ) : restr.nom_restr_t === "lactosa" ? (
                                                <span role="img" aria-label="Milk">🥛</span>
                                            ) : restr.nom_restr_t === "alcohol" ? (
                                                <span role="img" aria-label="Alcohol">🍷</span>
                                            ) : restr.nom_restr_t === "vegano" ? (
                                                <span role="img" aria-label="Vegan">🌱</span>
                                            ) : restr.nom_restr_t === "frutos secos" ? (
                                                <span role="img" aria-label="Peanut">🥜</span>
                                            ) : null}
                                            {restr.nom_restr_t}
                                        </p>
                                    ))
                                ) : (
                                    <p>No hay restricciones</p>
                                )}

                            </div>
                        </div>
                    </div>
                </div>
            ) : favoritos.length > 0 ? (
                favoritos.map((favorito) => {
                    const recetaKey = `${favorito.nom_receta_f}-${favorito.nom_usuario_r_f}-${favorito.fecha_creac_receta_f}`;
                    const receta = recetas[recetaKey];

                    return (
                        receta && (
                            <div className='receta-item' key={recetaKey}>
                                <p className="receta-nombre">{receta.nom_receta}</p>
                                <div className="imagen-descrip">
                                    <img src={receta.foto} alt={receta.nom_receta} />
                                    <div>
                                        <p className="descripcion">
                                            {receta.descripcion ? receta.descripcion.slice(0, 27) + "..." : "Sin descripción"}
                                        </p>
                                        <button onClick={() => handleVerMasReceta(receta)} className="ver-mas-btn">Ver más</button>
                                        <div className="div-pers-tiempo">
                                            <FontAwesomeIcon icon={faPeopleGroup} className='icono-ari'/>
                                            <p className='texto-pers-tiempo'>{receta.num_personas}</p>
                                            <FontAwesomeIcon icon={faClock} className='icono-ari'/>
                                            <p className='texto-pers-tiempo'>{receta.tiempo_prep}</p>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        )
                    );
                })
            ) : (
                <p>No hay recetas favoritas</p>
            )}
        </div>
    );
}
