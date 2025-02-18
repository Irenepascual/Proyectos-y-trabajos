'use client';
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import "/src/app/style-anadir-receta.css";
import "/src/app/style-ingredientes.css";

/*
* Devuelve la parte de la página de añadir receta donde se
* completan los campos para crear correctamente la receta.
* Los campos que se completarán serán: nombre receta, descripción,
* número de personas para los que está hecha, tiempo necesario 
* para cocinarla, enlace a imagen url de la receta, ingredientes 
* necesarios, detalles de los ingredientes (cantidad y tipo) y 
* se marca como aprobada directamente (por ser receta creada
* por el propietario).
*/
export default function AnadirReceta({ nomUsuario }) {
    const router = useRouter();
    const [paso, setPaso] = useState(1);

    const [nombreReceta, setNombreReceta] = useState('');
    const [descripcion, setDescripcion] = useState('');
    const [numPersonas, setNumPersonas] = useState(1);
    const [tiempo, setTiempo] = useState(1);
    const [imagenUrl, setImagenUrl] = useState(''); // Estado para la URL de la imagen

    const [ingredientes, setIngredientes] = useState([]);
    const [selectedIngredientes, setSelectedIngredientes] = useState([]);
    const [ingredienteDetalles, setIngredienteDetalles] = useState({});

    const [errorImagen, setErrorImagen] = useState(false); // Estado para mostrar el error si no se introduce una URL válida
    const [errorTitulo, setErrorTitulo] = useState(false); // Estado para mostrar el error si no se introduce un título
    const [errorCantidadUnidad, setErrorCantidadUnidad] = useState(false);

    const [showSuccessMessage, setShowSuccessMessage] = useState(false);
    const [showFailMessage, setShowFailMessage] = useState(false);
    
    {(imagenUrl) && (
        <div className="ar-en-fila">
            <p className="ar-titulo-formulario">Vista previa de la imagen:</p>
            <img
                src={imagenUrl}
                alt="Vista previa"
                className="ar-imagen-previa"
            />
        </div>
    )}

    const fetchIngredientes = async () => {
        const response = await fetch('/routes/ingredientes');
        const data = await response.json();
        setIngredientes(data);
    };

    const toggleIngrediente = (nom_ingred, nom_usuario_i) => {
        setSelectedIngredientes((prev) => {
            // Busca si ya existe un ingrediente con el mismo nom_ingred y nom_usuario_i
            const exists = prev.some(
                (item) => item.nom_ingred === nom_ingred && item.nom_usuario_i === nom_usuario_i
            );

            if (exists) {
                // Si el ingrediente ya está en selectedIngredientes, se quita
                return prev.filter(
                    (item) => !(item.nom_ingred === nom_ingred && item.nom_usuario_i === nom_usuario_i)
                );
            } else {
                // Si no está, se añade el ingrediente junto con el nombre de usuario
                return [...prev, { nom_ingred, nom_usuario_i }];
            }
        });
    };

    useEffect(() => {
        fetchIngredientes();
    }, []);

    const handleCantidadChange = (nom_ingred, nom_usuario_i, cantidad) => {
        setIngredienteDetalles((prev) => ({
            ...prev,
            [`${nom_ingred}-${nom_usuario_i}`]: {
                ...prev[`${nom_ingred}-${nom_usuario_i}`],
                cantidad,
            },
        }));
    };

    const handleUnidadChange = (nom_ingred, nom_usuario_i, unidad) => {
        setIngredienteDetalles((prev) => ({
            ...prev,
            [`${nom_ingred}-${nom_usuario_i}`]: {
                ...prev[`${nom_ingred}-${nom_usuario_i}`],
                unidad,
            },
        }));
    };

    const pasoAnterior = () => {
        if (paso === 4 && selectedIngredientes.length === 0) {
            setPaso(2);
        } else {
            setPaso(paso - 1);
        }
    };

    const pasoSiguiente = () => {
        // Validar que el título no esté vacío
        if (nombreReceta.trim() === '') {
            setErrorTitulo('El título es obligatorio.');
            return; // No avanzar si el título está vacío
        } else {
            setErrorTitulo(false); // Limpiar el error si el título es válido
        }
    
        // Validar la URL de la imagen
        if (imagenUrl.trim() === '') {
            setErrorImagen('La URL de la imagen es obligatoria.');
            return; // No avanzar si la URL de la imagen está vacía
        } else if (!esUrlValida(imagenUrl)) {
            setErrorImagen('La URL de la imagen no es válida.');
            return; // No avanzar si la URL no es válida
        } else {
            setErrorImagen(false); // Limpiar el error de la imagen
        }
    
        // Validación de los ingredientes en el paso 3
        if (paso === 3) {
            let cantidadUnidadCompleta = true;
            selectedIngredientes.forEach((selIngred) => {
                const key = `${selIngred.nom_ingred}-${selIngred.nom_usuario_i}`;
                const cantidad = ingredienteDetalles[key]?.cantidad;
                const unidad = ingredienteDetalles[key]?.unidad;
    
                if (!cantidad || !unidad) {
                    cantidadUnidadCompleta = false; // Si falta cantidad o unidad, no está completo
                }
            });
    
            // Si falta algún dato de cantidad o unidad, no pasar al siguiente paso
            if (!cantidadUnidadCompleta) {
                setErrorCantidadUnidad('Todos los ingredientes deben tener cantidad y unidad.');
                return;
            } else {
                setErrorCantidadUnidad(false); // Eliminar el error si todo está completo
            }
        }
    
        // Avanzar al siguiente paso si no hay errores
        setPaso(paso + 1);
    };
    

    // Función para cancelar el formulario
    const cancelarFormulario = () => {
        // Reinicia todos los campos y regresa al paso 1
        setPaso(1);
        setNombreReceta('');
        setDescripcion('');
        setNumPersonas(1);
        setTiempo(0);
        setImagenUrl('');
        setIngredientes([]);
        setSelectedIngredientes([]);
        setIngredienteDetalles([]);
    };

    const confirmarFormulario = async () => {
        try {
            const fecha = new Date().toISOString();
            console.log(fecha);
            const receta = {
                nombreReceta,
                nomUsuario,
                descripcion,
                numPersonas,
                tiempo,
                imagenUrl,
                fecha,
                estado: "aprobado",
            };
            const response = await fetch('/routes/recetas', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(receta),
            });

            if (response.ok) {
                console.log("Receta creada correctamente");

                if (selectedIngredientes.length === 0) {
                    // Mostrar el pop-up de éxito
                    setShowSuccessMessage(true);
                    // Ocultar el pop-up después de 5 segundos
                    setTimeout(() => {
                        setShowSuccessMessage(false);
                        router.push('/home-con-sesionP'); // Redirige después del pop-up
                    }, 5000);
                }

                // AÑADIMOS LOS INGREDIENTES QUE UTILIZA
                const ingredientesData = selectedIngredientes.map((selIngred) => {
                    const key = `${selIngred.nom_ingred}-${selIngred.nom_usuario_i}`;
                    const cantidad = ingredienteDetalles[key]?.cantidad;
                    const unidad = ingredienteDetalles[key]?.unidad;
    
                    // Creamos el objeto para cada ingrediente
                    // !data.nomUsuarioReceta || !data.nomReceta ||  
                    return {
                        nombreIngrediente: selIngred.nom_ingred,
                        nombreUsuario: selIngred.nom_usuario_i,
                        cantidad,
                        unidad,
                        fecha,
                        nomUsuario,
                        nombreReceta,
                    };
                });
    
                // Enviar cada ingrediente a la API
                for (const ingrediente of ingredientesData) {
                    const ingredienteResponse = await fetch('/routes/utilizaIngrediente', {
                        method: 'POST',
                        headers: {
                            'Content-Type': 'application/json',
                        },
                        body: JSON.stringify(ingrediente),
                    });
    
                    if (ingredienteResponse.ok) {
                        console.log(`Ingrediente ${ingrediente.nombreIngrediente} añadido correctamente.`);
                    } else {
                        console.error(`Error al añadir el ingrediente ${ingrediente.nombreIngrediente}`);
                    }
                }

                // Mostrar el pop-up de éxito
                setShowSuccessMessage(true);
                // Ocultar el pop-up después de 5 segundos
                setTimeout(() => {
                    setShowSuccessMessage(false);
                    router.push('/home-con-sesionP'); // Redirige después del pop-up
                }, 5000);

            } else {
                console.error("Error al crear la receta");
                // Mostrar el pop-up de fallo
                setShowFailMessage(true);
                // Ocultar el pop-up después de 5 segundos
                setTimeout(() => {
                    setShowFailMessage(false);
                    router.push('/home-con-sesion'); // Redirige después del pop-up
                }, 5000);
            }
        } catch(error) {
            console.error("Error:", error);
        }

    };

    const esUrlValida = (url) => {
        const patron = /^(ftp|http|https):\/\/[^ "]+$/;
        return patron.test(url);
    };

    return (
        <div>
            <h3 className="ar-titulo">CREAR UNA NUEVA RECETA:</h3>

            {showSuccessMessage && (
                <div className="popup-OK">
                    <p>¡RECETA CREADA CORRECTAMENTE!</p>
                    <p>Tu receta ya está disponible en la página principal</p>
                    <p>Támbien puedes consultar tu receta en el apartado "Mis Recetas"</p>
                </div>
            )}

            {showFailMessage && (
                <div className="popup-OK">
                    <p>ERROR AL CREAR LA RECETA</p>
                </div>
            )}

            {paso === 1 && (
                <div className="ar-general">
                    <p className="ar-texto-pasos">Paso 1: Introduce los datos de la receta:</p>
                    <div className="ar-en-fila">
                        <p className="ar-titulo-formulario">Nombre de la receta:</p>
                        <input
                            className="ar-formulario"
                            type="text"
                            value={nombreReceta}
                            onChange={(e) => setNombreReceta(e.target.value)}
                            placeholder="Nombre"
                        />
                        {errorTitulo && <p className="ar-error">{errorTitulo}</p>} {/* Mensaje de error para el título */}
                    </div>
                    <div className="ar-en-fila">
                        <p className="ar-titulo-formulario">Descripción:</p>
                        <textarea
                            className="ar-descripcion"
                            value={descripcion}
                            onChange={(e) => setDescripcion(e.target.value)}
                            placeholder="Explica los pasos"
                        />
                    </div>
                    <div className="ar-en-fila">
                        <p className="ar-titulo-formulario">Número de personas:</p>
                        <input
                            className="ar-formulario-pequeno"
                            type="number"
                            value={numPersonas}
                            onChange={(e) => setNumPersonas(e.target.value)}
                            min="1"
                        />
                    </div>
                    <div className="ar-en-fila">
                        <p className="ar-titulo-formulario">Tiempo de preparación:</p>
                        <input
                            className="ar-formulario-pequeno"
                            type="number"
                            value={tiempo}
                            onChange={(e) => setTiempo(e.target.value)}
                            min="1"
                            placeholder="Minutos"
                        /> minutos
                    </div>
                    <div className="ar-en-fila">
                        <p className="ar-titulo-formulario">Añadir imagen (URL):</p>
                        <input
                            type="text"
                            className="ar-formulario"
                            value={imagenUrl}
                            onChange={(e) => setImagenUrl(e.target.value)}
                            placeholder="Pega el enlace de la imagen"
                        />
                        {errorImagen && <p className="ar-error">{errorImagen}</p>} {/* Mensaje de error para la URL de la imagen */}
                    </div>
                </div>
            )}
 
            {paso === 2 && (
                <div className="ar-general">
                    <p className="ar-texto-pasos">Paso 2: Selecciona los ingredientes que contiene tu receta:</p>
                    <div className="contenedor-ingredientes">
                        {ingredientes.length > 0 ? (
                            ingredientes.map((ingred) => {
                                const isSelected = selectedIngredientes.some(
                                    (selected) => selected.nom_ingred === ingred.nom_ingred && selected.nom_usuario_i === ingred.nom_usuario_i
                                );

                                return (
                                    <div
                                        key={`${ingred.nom_ingred}-${ingred.nom_usuario_i}`}
                                        className={`ingredientes ${isSelected ? 'selected' : ''}`}
                                        onClick={() => toggleIngrediente(ingred.nom_ingred, ingred.nom_usuario_i)}
                                    >
                                        <p>{ingred.nom_ingred}</p>
                                    </div>
                                );
                            })
                        ) : (
                            <p>No hay ingredientes disponibles</p>
                        )}

                    </div>
                </div>
            )}

            {paso === 3 && (
                <div className="ar-general">
                    <p className="ar-texto-pasos">Paso 3: Selecciona la cantidad de cada ingrediente:</p>

                    {errorCantidadUnidad && <p className="ar-error">{errorCantidadUnidad}</p>}

                    <div>
                        {selectedIngredientes.length > 0 ? (
                            selectedIngredientes.map((selIngred) => {
                                const key = `${selIngred.nom_ingred}-${selIngred.nom_usuario_i}`;
                                return (
                                    <div className="ar-contenedor-ingredientes" key={key}>
                                        <div className="ar-ingrediente">
                                            <p>{selIngred.nom_ingred}</p>
                                        </div>
                                        <div>
                                            <input
                                                className="ar-formulario-pequeno"
                                                type="number"
                                                value={ingredienteDetalles[key]?.cantidad || ''}
                                                onChange={(e) =>
                                                    handleCantidadChange(selIngred.nom_ingred, selIngred.nom_usuario_i, e.target.value)
                                                }
                                                min="1"
                                                placeholder="Cantidad"
                                                required
                                            />
                                            <select
                                                value={ingredienteDetalles[key]?.unidad || ''}
                                                onChange={(e) =>
                                                    handleUnidadChange(selIngred.nom_ingred, selIngred.nom_usuario_i, e.target.value)
                                                }
                                                className="ar-unidad-medida"
                                                required
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
                                    </div>
                                ); 
                            })
                        ) : (
                            <p>No hay ingredientes disponibles</p>
                        )}
                    </div>
                </div>
            )}

            {paso === 4 && (
                <div className="ar-general">
                    <p className="ar-texto-pasos">Paso 4: Comprueba que los datos introducidos sean correctos:</p>
                    <div className="ar-en-fila">
                        <img className="ar-imagen" src={imagenUrl} alt={nombreReceta}/>
                        <div>
                            <p>Nombre de la receta: {nombreReceta}</p>
                            <p>Descripción: {descripcion}</p>
                            <p>Número de personas: {numPersonas}</p>
                            <p>Tiempo de preparación: {tiempo}</p>
                            
                            
                            {selectedIngredientes.length > 0 ? (
                                <ul className="ar-lista-ingredientes">
                                    {selectedIngredientes.map((selIngred) => {
                                        const key = `${selIngred.nom_ingred}-${selIngred.nom_usuario_i}`;
                                        const ingredienteDetalle = ingredienteDetalles[key]; // Obtener el detalle del ingrediente

                                        return (
                                            <li key={key}>
                                                <p>
                                                    {selIngred.nom_ingred} 
                                                    <span> {ingredienteDetalle?.cantidad || '-'}</span> 
                                                    <span> {ingredienteDetalle?.unidad || '-'}</span>
                                                </p>
                                            </li>
                                        );
                                    })}
                                </ul>
                            ) : (
                                <p>No hay ingredientes seleccionados</p>
                            )}
                        </div>
                    </div>
                </div>
            )}

            <div className="ar-pasos-indicador">
                {paso === 1 ? ( 
                    <><button onClick={cancelarFormulario} className="ar-boton1">Cancelar</button></>
                ) : (
                    <><button onClick={pasoAnterior} className="ar-boton2">Anterior</button></>
                )}
                        
                {[1, 2, 3, 4].map((num) => (
                    <div
                        key={num}
                        className={`ar-paso-circulo ${paso === num ? 'activo' : ''}`}
                        // onClick={() => setPaso(num)} // Opcional: para permitir que el usuario haga clic en un paso para ir a él
                    >
                        {num}
                    </div>
                ))}

                {paso === 4 ? ( 
                    <><button onClick={confirmarFormulario} className="ar-boton1">Confirmar</button></>
                ) : (
                    <><button onClick={pasoSiguiente} className="ar-boton2">Siguiente</button></>
                )}
            </div>
        </div>
    );
}