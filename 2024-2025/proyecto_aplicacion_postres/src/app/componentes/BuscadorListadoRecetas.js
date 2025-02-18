import React, { useEffect, useState } from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faPeopleGroup, faClock, faMagnifyingGlass, faHeart } from '@fortawesome/free-solid-svg-icons';
import { faHeart as faHeartRegular } from '@fortawesome/free-regular-svg-icons';
import "/src/app/style-recetas.css";
import "/src/app/style-buscador.css";
import "/src/app/style-ingredientes.css";
import "/src/app/style-recetaUnica.css";

/*
* Devuelve el componente RecetasList, donde se cargan las
* recetas, ingredientes y restricciones, se permite la 
* búsqueda por nombre y filtrado de restricciones y 
* gestiona los favoritos (añadir o eliminar una receta
* al usuario que esté con la sesión iniciada).
*/
export default function RecetasList() {
    const [search, setSearch] = useState("");
    const [isLoading, setIsLoading] = useState(true);

    const [todasRecetas, setTodasRecetas] = useState([]);
    const [recetas, setRecetas] = useState([]);
    const [selectedReceta, setSelectedReceta] = useState(null);
    const [selectedRecetaKey, setSelectedRecetaKey] = useState(null);

    const [ingredientes, setIngredientes] = useState([]);
    const [selectedIngredientes, setSelectedIngredientes] = useState([]);
    const [utiliza, setUtiliza] = useState([]);

    const [favoritos, setFavoritos] = useState([]);

    const [restricciones, setRestricciones] = useState([]);
    const [selectedRestricciones, setSelectedRestricciones] = useState([]);

    const handleVerMasReceta = (receta) => {
        // Establecemos la receta seleccionada
        const recetaKey = `${receta.nom_usuario_r}-${receta.nom_receta}-${receta.fecha_creac_receta}`;
        setSelectedReceta(receta);
        setSelectedRecetaKey(recetaKey); // Guardamos la clave para acceder a las restricciones
    };
    
    const handleCloseDetails = () => {
        setSelectedReceta(null);
    };

    const fetchIngredientes = async () => {
        try {
            const response = await fetch('/routes/ingredientes');
            const data = await response.json();
            setIngredientes(data);
        } catch (error) {
            console.error("Error al obtener los ingredientes:", error);
        }
    };

    const fetchRecetas = async (query = "") => {
        try {
            const url = query ? `/routes/recetas?nom_receta=${query}` : '/routes/recetas?estado=aprobado';
            const response = await fetch(url);
            if (!response.ok) throw new Error("Error al obtener las recetas");
            const data = await response.json();
            setRecetas(data);
            setTodasRecetas(data);
        } catch (error) {
            console.error("Error al obtener las recetas:", error);
        }
    };

    const fetchFavoritoByReceta = async (nom_usuario, nom_usuario_r, nom_receta, fecha_creac_receta) => {
        try {
            const url = `/routes/favoritos?nom_usuario=${nom_usuario}&nom_usuario_r=${nom_usuario_r}&nom_receta=${nom_receta}&fecha_creac_receta=${fecha_creac_receta}`;
            const response = await fetch(url);
            if (!response.ok) throw new Error("Error al obtener si la receta es o no favorita");
            const data = await response.json();
            setFavoritos((fav) => ({
                ...fav,
                [`${nom_usuario_r}-${nom_receta}-${fecha_creac_receta}`]: data,
            }));
        } catch (error) {
            console.error("Error al obtener si la receta es o no favorita:", error);
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
    


    const toggleFavorito = async (receta) => {
        const storedNomUsuario = localStorage.getItem('nom_usuario');
        const favoritoKey = `${receta.nom_usuario_r}-${receta.nom_receta}-${receta.fecha_creac_receta}`;
        const isFavorito = favoritos[favoritoKey];

        try {
            if (isFavorito) {
                await fetch('/routes/favoritos', {
                    method: 'DELETE',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({
                        nom_usuario: storedNomUsuario,
                        nom_usuario_r: receta.nom_usuario_r,
                        nom_receta: receta.nom_receta,
                        fecha_creac_receta: receta.fecha_creac_receta
                    })
                });
            } else {
                await fetch('/routes/favoritos', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({
                        nom_usuario: storedNomUsuario,
                        nom_usuario_r: receta.nom_usuario_r,
                        nom_receta: receta.nom_receta,
                        fecha_creac_receta: receta.fecha_creac_receta
                    })
                });
            }
            setFavoritos((prevFavoritos) => ({
                ...prevFavoritos,
                [favoritoKey]: !isFavorito
            }));
        } catch (error) {
            console.error("Error al actualizar los favoritos:", error);
        }
    };

    const searcher = (e) => {
        const query = e.target.value;
        setSearch(query);
        fetchRecetas(query);
    };

    const filterRecetas = () => {
        const recetasFiltradas = todasRecetas.filter((receta) => {
            const key = `${receta.nom_usuario_r}-${receta.nom_receta}-${receta.fecha_creac_receta}`;
    
            // Obtener las restricciones asociadas a la receta
            const restriccionesReceta = restricciones[key] ? restricciones[key].map((restr) => restr.nom_restr_t) : [];
    
            // Lógica específica para "vegano"
            if (selectedRestricciones.includes("vegano")) {
                // Mostrar solo recetas que sean veganas
                if (!restriccionesReceta.includes("vegano")) return false;
            } else {
                // Mostrar recetas que no tengan la restricción seleccionada
                const tieneRestriccion = selectedRestricciones.some((restriccion) => restriccionesReceta.includes(restriccion));
                if (tieneRestriccion) return false;
            }
    
            // Verificar los ingredientes
            const ingredientesReceta = utiliza[key]?.map((ingred) => ingred.nom_ingred_ut) || [];
            return selectedIngredientes.every((ingred) => ingredientesReceta.includes(ingred));
        });
    
        setRecetas(recetasFiltradas);
    };

    const toggleIngrediente = (ingrediente) => {
        setSelectedIngredientes((prev) => {
            const updated = prev.includes(ingrediente) ? prev.filter((ing) => ing !== ingrediente) : [...prev, ingrediente];
            return updated;
        });
    };

    const toggleRestriccion = (restriccion) => {
        setSelectedRestricciones((prev) => {
            const updated = prev.includes(restriccion) ? prev.filter((restr) => restr !== restriccion) : [...prev, restriccion];
            return updated;
        });
    };

    useEffect(() => {
        const loadData = async () => {
            setIsLoading(true);
            await fetchIngredientes();
            await fetchRecetas();
            setIsLoading(false);
        };
        loadData();
    }, []);

    useEffect(() => {
        const storedNomUsuario = localStorage.getItem('nom_usuario');
        recetas.forEach((receta) => {
            fetchFavoritoByReceta(storedNomUsuario, receta.nom_usuario_r, receta.nom_receta, receta.fecha_creac_receta);
            fetchUtiliza(receta.nom_usuario_r, receta.nom_receta, receta.fecha_creac_receta);
        });
    }, [recetas]);

    useEffect(() => {
        filterRecetas();
    }, [selectedIngredientes, selectedRestricciones]);

    const [showAllIngredientes, setShowAllIngredientes] = useState(false);

    const toggleShowAllIngredientes = () => {
        setShowAllIngredientes((prev) => !prev);
    };

    return (
        <main>
            {isLoading ? (
                <div className="loading-container">
                    <div className="spinner"></div>
                </div>
            ) : selectedReceta ? ( 
                <div>
                    <button onClick={handleCloseDetails} className="u-boton-volver">Volver</button>
                    <h2 className="u-nom-receta">{selectedReceta.nom_receta}</h2>
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
                                {utiliza[`${selectedReceta.nom_usuario_r}-${selectedReceta.nom_receta}-${selectedReceta.fecha_creac_receta}`] && utiliza[`${selectedReceta.nom_usuario_r}-${selectedReceta.nom_receta}-${selectedReceta.fecha_creac_receta}`].length > 0 ? (
                                    <ul className="u-ingred-lista">
                                        {utiliza[`${selectedReceta.nom_usuario_r}-${selectedReceta.nom_receta}-${selectedReceta.fecha_creac_receta}`].map((ingr) => (
                                            <li key={`${ingr.nom_ingred_ut}-${ingr.nom_receta_ut}-${ingr.nom_usuario_r_ut}-${ingr.fecha_creac_receta_ut}`} className="u-ingred-lista-item">
                                                {ingr.cantidad} {ingr.unidad_med} de {ingr.nom_ingred_ut}
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
                                {restricciones[selectedRecetaKey] && restricciones[selectedRecetaKey].map((restr) => (
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
                                ))}
                            </div>
                        </div>
                    </div>
                </div>
            ) : (
                <>
                    <div className="contenedor-buscador">
                        <input
                            value={search}
                            onChange={searcher}
                            type="text"
                            placeholder="Buscar por nombre de receta"
                            className="buscador"
                        />
                        <FontAwesomeIcon icon={faMagnifyingGlass} className="search-icon" />
                    </div>
                    <div className="contenedor-busq-restriccion">
                        <div className={`boton-restriccion ${selectedRestricciones.includes("gluten") ? '-pulsado' : ''}`}
                            onClick={() => toggleRestriccion("gluten")}>
                            <span role="img" aria-label="Gluten" className="busq-emoji">🌾</span>
                            <p className="busq-texto">Sin gluten</p>
                        </div>
                        <div className={`boton-restriccion ${selectedRestricciones.includes("huevo") ? '-pulsado' : ''}`}
                            onClick={() => toggleRestriccion("huevo")}>
                            <span role="img" aria-label="Egg" className="busq-emoji">🥚</span>
                            <p className="busq-texto">Sin huevo</p>
                        </div>
                        <div className={`boton-restriccion ${selectedRestricciones.includes("lactosa") ? '-pulsado' : ''}`}
                            onClick={() => toggleRestriccion("lactosa")}>
                            <span role="img" aria-label="Milk" className="busq-emoji">🥛</span>
                            <p className="busq-texto">Sin lactosa</p>
                        </div>
                        <div className={`boton-restriccion ${selectedRestricciones.includes("frutos secos") ? '-pulsado' : ''}`}
                            onClick={() => toggleRestriccion("frutos secos")}>
                            <span role="img" aria-label="Peanut" className="busq-emoji">🥜</span>
                            <p className="busq-texto">Sin frutos secos</p>
                        </div>
                        <div className={`boton-restriccion ${selectedRestricciones.includes("alcohol") ? '-pulsado' : ''}`}
                            onClick={() => toggleRestriccion("alcohol")}>
                            <span role="img" aria-label="Alcohol" className="busq-emoji">🍷</span>
                            <p className="busq-texto">Sin alcohol</p>
                        </div>
                        <div className={`boton-restriccion ${selectedRestricciones.includes("vegano") ? '-pulsado' : ''}`}
                            onClick={() => toggleRestriccion("vegano")}>
                            <span role="img" aria-label="Vegan" className="busq-emoji">🌱</span>
                            <p className="busq-texto">Vegano</p>
                        </div>
                    </div>
                    
                    <div className="contenedor-ingredientes">
                        <p className="texto-seleccionar-ingredientes">Selecciona los ingredientes que quieras que incluya la receta:</p>
                        <div>
                        {ingredientes.length > 0 ? (
                            <>
                                {ingredientes.slice(0, showAllIngredientes ? ingredientes.length : 13).map((ingred) => (
                                    <div
                                        key={ingred.nom_ingred}
                                        className={`ingredientes ${selectedIngredientes.includes(ingred.nom_ingred) ? 'selected' : ''}`}
                                        onClick={() => toggleIngrediente(ingred.nom_ingred)}
                                    >
                                        <p>{ingred.nom_ingred}</p>
                                    </div>
                                ))}

                                {ingredientes.length > 13 && (
                                    <button className="mostrar-mas-btn" onClick={toggleShowAllIngredientes}>
                                        {showAllIngredientes ? "Mostrar menos ingredientes" : "Mostrar más ingredientes"}
                                    </button>
                                )}
                            </>
                        ) : (
                            <p>No hay ingredientes disponibles</p>
                        )}
                        </div>
                    </div>

                    <div className="grid-container">
                        {recetas.length > 0 ? (
                            recetas.map((receta) => {
                            const descripcionCorta = receta.descripcion.length > 23 
                                ? receta.descripcion.slice(0, 27) + "..." 
                                : receta.descripcion;
                            const key = `${receta.nom_usuario_r}-${receta.nom_receta}-${receta.fecha_creac_receta}`;
                            const favorito = favoritos[key];
                            const ut = utiliza[key];

                            return (
                                <div className='receta-item' key={`${receta.nom_receta}-${receta.nom_usuario_r}`}>
                                    <p className="receta-nombre">{receta.nom_receta}</p>
                                    <div className="imagen-descrip">
                                        <div>
                                            <img src={receta.foto} alt={receta.nom_receta} />
                                            <div className="en-fila">
                                                {restricciones[`${receta.nom_usuario_r}-${receta.nom_receta}-${receta.fecha_creac_receta}`] && restricciones[`${receta.nom_usuario_r}-${receta.nom_receta}-${receta.fecha_creac_receta}`].map((restr) => (
                                                    <p key={restr.nom_restr_t} className="emoji">
                                                        {restr.nom_restr_t === "huevo" ? (
                                                            <span role="img" aria-label="Egg">🥚</span>
                                                            // <FontAwesomeIcon icon={faEgg} />
                                                        ) : restr.nom_restr_t === "gluten" ? (
                                                            <span role="img" aria-label="Gluten">🌾</span>
                                                            // <FontAwesomeIcon icon={faWheatAwn} />
                                                        ) : restr.nom_restr_t === "lactosa" ? (
                                                            // <FontAwesomeIcon icon={faGlassWater} />
                                                            <span role="img" aria-label="Milk">🥛</span>
                                                        ) : restr.nom_restr_t === "alcohol" ? (
                                                            <span role="img" aria-label="Alcohol">🍷</span>
                                                            // <FontAwesomeIcon icon={faMartiniGlassCitrus} />
                                                        ) : restr.nom_restr_t === "vegano" ? (
                                                            <span role="img" aria-label="Vegan">🌱</span>
                                                            // <FontAwesomeIcon icon={faLeaf} />
                                                        ) : restr.nom_restr_t === "frutos secos" ? (
                                                            <span role="img" aria-label="Peanut">🥜</span>
                                                        ) : null}
                                                    </p>
                                                ))}
                                            </div>
                                        </div>
                                        <div>
                                            <p className="descripcion">{descripcionCorta}</p>
                                            <button onClick={() => handleVerMasReceta(receta)} className="ver-mas-btn">Ver más</button>
                                            <div className="div-pers-tiempo">
                                                <FontAwesomeIcon icon={faPeopleGroup} className='icono-ari'/>
                                                <p className='texto-pers-tiempo'>{receta.num_personas}</p>
                                                <FontAwesomeIcon icon={faClock} className='icono-ari'/>
                                                <p className='texto-pers-tiempo'>{receta.tiempo_prep}</p>
                                            </div>
                                            <div className="ingred-utiliza-container">
                                                {ut && ut.length > 0 ? (
                                                    <>
                                                        {ut.slice(0, 4).map((ingr, index) => (
                                                            <div key={`${ingr.nom_ingred_ut}-${key}-${index}`}>
                                                                <p className="ingred-utiliza">{ingr.nom_ingred_ut}</p>
                                                            </div>
                                                        ))}
                                                        {ut.length > 4 && (
                                                            <p className="ingred-utiliza">...</p> // Indicador de más ingredientes
                                                        )}
                                                    </>
                                                ) : (
                                                    <p className="ingred-utiliza">No hay ingredientes</p>
                                                )}
                                            </div>
                                        </div>
                                    </div>
                                    <FontAwesomeIcon
                                        icon={favorito ? faHeart : faHeartRegular}
                                        className='icono-corazon'
                                        onClick={() => toggleFavorito(receta)}
                                    />
                                </div>
                            );
                        })) : (
                            <p>No hay recetas disponibles</p>
                        )}
                    </div>
                </>
            )}
        </main>
    );
}
