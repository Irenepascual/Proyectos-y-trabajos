'use client';
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faPeopleGroup, faClock } from '@fortawesome/free-solid-svg-icons'; // Asegúrate de que estos iconos estén importados
import "/src/app/style-recetas.css";
import "/src/app/style-recetaUnica.css";
import "/src/app/style-modificar-receta.css";

/*
* Permite gestionar a un propietario sus recetas, donde
* puede verlas y modificarlas. Al seleccionar una receta, 
* se muestra su información (descripción, ingredientes, 
* restricciones). Al modificar una receta, se permite 
* editar su descripción, el número de personas, el tiempo
* de preparación y la imagen. 
* Cuando se guarden los cambios, se actualizará la base
* de datos mediante las solicitudes API. 
*/
export default function MisRecetas({ nomUsuario }) {
    const router = useRouter();
    const [loading, setLoading] = useState(true);

    const [recetas, setRecetas] = useState([]);
    const [selectedReceta, setSelectedReceta] = useState(null);
    const [selectedRecetaKey, setSelectedRecetaKey] = useState(null);
    const [modificarReceta, setModificarReceta] = useState(null);
    const [modificarRecetaKey, setModificarRecetaKey] = useState(null);

    const [utiliza, setUtiliza] = useState([]);
    const [restricciones, setRestricciones] = useState([]);

    // NUEVO PARA MODIFICAR LA RECETA
    const [ingredienteDetalles, setIngredienteDetalles] = useState({});

    const [nombreReceta, setNombreReceta] = useState('');
    const [descripcion, setDescripcion] = useState('');
    const [numPersonas, setNumPersonas] = useState(1);
    const [tiempo, setTiempo] = useState(1);
    const [imagenUrl, setImagenUrl] = useState(''); // Estado para la URL de la imagen

    const [errorImagen, setErrorImagen] = useState(false); // Estado para mostrar el error si no se introduce una URL válida

    const [showSuccessMessage, setShowSuccessMessage] = useState(false);
    const [showFailMessage, setShowFailMessage] = useState(false);
    //

    const cambiarUtiliza = (cantidad, nomUsuarioReceta, nomReceta, fecha, nomIngrediente, nomUsuarioIngrediente) => {
        const key = `${nomUsuarioReceta}-${nomReceta}-${fecha}`;
    
        setUtiliza((prevUtiliza) => ({
            ...prevUtiliza,
            [key]: prevUtiliza[key].map((ingred) =>
                ingred.nom_ingred_ut === nomIngrediente && ingred.nom_usuario_i_ut === nomUsuarioIngrediente
                    ? { ...ingred, cantidad } // Solo modifica la cantidad del ingrediente específico
                    : ingred
            ),
        }));
    };
    
    const handleVerMasReceta = (receta) => {
        // Establecemos la receta seleccionada
        const recetaKey = `${receta.nom_usuario_r}-${receta.nom_receta}-${receta.fecha_creac_receta}`;
        setSelectedReceta(receta);
        setSelectedRecetaKey(recetaKey);

        setNombreReceta(receta.nom_receta);
    };

    const handleCloseDetails = () => {
        setSelectedReceta(null);
        setModificarReceta(null);
    };

    const handleModificarReceta = (receta) => {
        // Establecemos la receta seleccionada
        const modificarKey = `${receta.nom_usuario_r}-${receta.nom_receta}-${receta.fecha_creac_receta}`;
        setModificarReceta(receta);
        setModificarRecetaKey(modificarKey);
    };

    useEffect(() => {
        if (modificarReceta) {
            setNombreReceta(modificarReceta.nom_receta || '');  // Pre-completamos el nombre si modificarReceta existe
            setDescripcion(modificarReceta.descripcion || '');  // Pre-completamos la descripción
            setNumPersonas(modificarReceta.num_personas);  // Pre-completamos el número de personas
            setTiempo(modificarReceta.tiempo_prep);        // Pre-completamos el tiempo de preparación
            setImagenUrl(modificarReceta.foto || '');           // Pre-completamos la imagen si existe
        }
    }, [modificarReceta]);

    const fetchRecetas = async (query) => {
        try {
            const url = `/routes/recetas?nom_usuario=${query}`;
            const response = await fetch(url);
            if (!response.ok) throw new Error("Error al obtener las recetas del usuario");
            const data = await response.json();
            setRecetas(data);
        } catch (error) {
            console.error("Error al obtener las recetas del usuario:", error);
        } finally {
            setLoading(false);
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
        if (nomUsuario) fetchRecetas(nomUsuario);
    }, [nomUsuario]);

    useEffect(() => {
        recetas.forEach((receta) => {
            fetchUtiliza(receta.nom_usuario_r, receta.nom_receta, receta.fecha_creac_receta);
        });
    }, [recetas]);

    const guardarCambios = async () => {
        const fecha = modificarReceta.fecha_creac_receta;
        const usuarioReceta = modificarReceta.nom_usuario_r;
        const receta = {
            nom_receta: nombreReceta,
            nom_usuario_r: usuarioReceta,
            fecha_creac_receta: fecha,
            descripcion: descripcion,
            foto: imagenUrl,
            tiempo_prep: tiempo,
            num_personas: numPersonas,
            estado: "aprobado",
        };
        const response = await fetch('/routes/recetas', {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(receta),
        });
        if (!response.ok) {
            console.error('Error al modificar la receta', response.error);
            // Mostrar el pop-up de fallo
            setShowFailMessage(true);
            // Ocultar el pop-up después de 5 segundos
            setTimeout(() => {
                setShowFailMessage(false);
                router.push('/home-con-sesion'); // Redirige después del pop-up
            }, 3000);
        }
        else {
            console.log('Receta modificada correctamente');
            // AÑADIMOS LOS INGREDIENTES QUE UTILIZA
            const keyUtiliza = `${modificarReceta.nom_usuario_r}-${modificarReceta.nom_receta}-${modificarReceta.fecha_creac_receta}`;
            const ingredientesData = utiliza[keyUtiliza].map((ingred) => {
                const keyU = `${ingred.nom_ingred_ut}-${ingred.nom_usuario_i_ut}`;
                return {
                    nombreIngrediente: ingred.nom_ingred_ut,
                    nombreUsuario: ingred.nom_usuario_i_ut,
                    cantidad: ingred.cantidad,
                    unidad: ingred.unidad_med,
                    nombreReceta: modificarReceta.nom_receta,
                    nomUsuarioReceta: modificarReceta.nom_usuario_r,
                    fechaReceta: modificarReceta.fecha_creac_receta,
                };
            });
            // Enviar cada ingrediente a la API
            for (const ingrediente of ingredientesData) {
                const ingredienteResponse = await fetch('/routes/utilizaIngrediente', {
                    method: 'PUT',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    body: JSON.stringify(ingrediente),
                });
                if (ingredienteResponse.ok) {
                    console.log(`Ingrediente ${ingrediente.nombreIngrediente} modificado correctamente.`);
                } else {
                    console.error(`Error al modificar el ingrediente ${ingrediente.nombreIngrediente}`);
                }
            }
            // Mostrar el pop-up de éxito
            setShowSuccessMessage(true);
            // Ocultar el pop-up después de 5 segundos
            setTimeout(() => {
                setShowSuccessMessage(false);
                router.push('/home-con-sesionP'); // Redirige después del pop-up
            }, 3000);
        }
    }

    return (
        <div>
            {showSuccessMessage && (
                <div className="popup-OK">
                    <p>¡RECETA MODIFICADA CORRECTAMENTE!</p>
                </div>
            )}

            {showFailMessage && (
                <div className="popup-OK">
                    <p>ERROR AL MODIFICAR LA RECETA</p>
                </div>
            )}

            {loading ? (
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
            ) : modificarReceta ? (
                <div>
                    <div className="mr-en-fila">
                        <p className="mr-titulo-formulario">Nombre de la receta:</p>
                        <p>{nombreReceta}</p>
                    </div>
                    <div className="mr-en-fila">
                        <p className="mr-titulo-formulario">Descripción:</p>
                        <textarea
                            className="mr-descripcion"
                            defaultValue={descripcion || ''}
                            onChange={(e) => setDescripcion(e.target.value)}
                            placeholder="Explica los pasos"
                        />
                    </div>
                    <div className="mr-en-fila">
                        <p className="mr-titulo-formulario">Número de personas:</p>
                        <input
                            className="mr-formulario-pequeno"
                            type="number"
                            value={numPersonas || 1}
                            onChange={(e) => setNumPersonas(e.target.value)}
                            min="1"
                            placeholder="Personas"
                        />
                    </div>
                    <div className="mr-en-fila">
                        <p className="mr-titulo-formulario">Tiempo de preparación:</p>
                        <input
                            className="mr-formulario-pequeno"
                            type="number"
                            value={tiempo}
                            onChange={(e) => setTiempo(e.target.value)}
                            placeholder="Minutos"
                        /> minutos
                    </div>
                    <div className="mr-en-fila">
                        <p className="mr-titulo-formulario">Imagen (URL):</p>
                        <input
                            type="text"
                            className="mr-formulario"
                            defaultValue={imagenUrl}
                            onChange={(e) => setImagenUrl(e.target.value)}
                            placeholder="Pega el enlace de la imagen"
                        />
                        {errorImagen && <p className="mr-error">{errorImagen}</p>} {/* Mensaje de error para la URL de la imagen */}
                    </div>
                    <div className="u-ingred-utiliza-container">
                        <p className="u-tit">Ingredientes añadidos:</p>
                        {utiliza[`${modificarReceta.nom_usuario_r}-${modificarReceta.nom_receta}-${modificarReceta.fecha_creac_receta}`] ? (
                            <ul className="u-ingred-lista">
                                {utiliza[`${modificarReceta.nom_usuario_r}-${modificarReceta.nom_receta}-${modificarReceta.fecha_creac_receta}`].map((ingrediente) => {
                                    const key = `${ingrediente.nom_ingred_ut}-${ingrediente.nom_usuario_i_ut}`;
                                    return (
                                        <div className="mr-contenedor-ingredientes" key={key}>
                                            <div className="mr-ingrediente">
                                                <p>{ingrediente.nom_ingred_ut}</p>
                                            </div>
                                            <input
                                                className="mr-formulario-pequeno"
                                                type="number"
                                                defaultValue={ingrediente.cantidad || ''}
                                                onChange={(e) => cambiarUtiliza(e.target.value, modificarReceta.nom_usuario_r, modificarReceta.nom_receta, modificarReceta.fecha_creac_receta, ingrediente.nom_ingred_ut, ingrediente.nom_usuario_i_ut)}
                                                min="1"
                                                placeholder="Cantidad"
                                                required
                                            />
                                            <select
                                                value={ingrediente.unidad_med || ''}
                                                className="mr-unidad-medida"
                                                required
                                                disabled
                                            >
                                                <option value="">Selecciona unidad</option>
                                                <option value="gr">gr</option>
                                                <option value="kg">kg</option>
                                                <option value="l">l</option>
                                                <option value="ml">ml</option>
                                                <option value="unidades">unidades</option>
                                                <option value="cucharada">cucharada</option>
                                                <option value="pizca">pizca</option>
                                                <option value="sobre">sobre</option>
                                                <option value="lata">lata</option>
                                            </select>
                                        </div>
                                    );
                                })}
                            </ul>
                        ) : (
                            <p>No hay ingredientes</p>
                        )}
                    </div>
                    <button onClick={guardarCambios} className="mr-boton-guardar">Guardar cambios</button>
                </div>
            ) : recetas.length > 0 ? (
                <div className="grid-container">
                    {recetas.map((receta) => {
                        const descripcionCorta = receta.descripcion.length > 23 
                            ? receta.descripcion.slice(0, 27) + "..." 
                            : receta.descripcion;
            
                        return (
                            <div className="receta-item" key={`${receta.nom_receta}-${receta.nom_usuario_r}`}>
                                <p className="receta-nombre">{receta.nom_receta}</p>
                                <div className="imagen-descrip">
                                    <img src={receta.foto} alt={receta.nom_receta} />
                                    <div>
                                        <p className="descripcion">{descripcionCorta}</p>
                                        <button onClick={() => handleVerMasReceta(receta)} className="ver-mas-btn">Ver más</button>
                                        <button onClick={() => handleModificarReceta(receta)} className="ver-mas-btn">Modificar</button>
                                        <div className="div-pers-tiempo">
                                            <FontAwesomeIcon icon={faPeopleGroup} className="icono-ari"/>
                                            <p className="texto-pers-tiempo">{receta.num_personas}</p>
                                            <FontAwesomeIcon icon={faClock} className="icono-ari"/>
                                            <p className="texto-pers-tiempo">{receta.tiempo_prep}</p>
                                        </div>
                                        <p className="aprobado">
                                        {receta.estado === "aprobado" 
                                            ? "Aprobada" 
                                        : receta.estado === "rechazado" 
                                                ? "Rechazada" 
                                        : "Sin aprobar"}
                                        </p>
                                    </div>
                                </div>
                            </div>
                        );
                    })}
                </div>
            ) : (
                <p>El usuario no tiene ninguna receta</p>
            )}
        
        </div>
    );
}
