"use client";
import "/src/app/style-peticiones.css";
import React, { useState, useEffect } from "react";
import HeaderConSesionP from "/src/app/componentes/HeaderConSesionP";
import { useRouter } from "next/navigation";

// Crea una interfaz para que los usuarios gestionen y respondan a las dudas no respondidas. 
// Permite a los usuarios ver las dudas, abrir un popup para responderlas, y actualizar automaticamente 
// la lista de dudas cada 5 segundos. Ademas, maneja el inicio y cierre de sesion, y proporciona 
// navegacion entre varias paginas de la aplicacion.

function Peticiones() {
    const [dudas, setDudas] = useState([]); 
    const [nomUsuario, setNomUsuario] = useState("");
    const [showPopup, setShowPopup] = useState(false);
    const [selectedDuda, setSelectedDuda] = useState(null);
    const [respuesta, setRespuesta] = useState("");
    const router = useRouter();

    useEffect(() => {
        const storedNomUsuario = localStorage.getItem("nom_usuario");
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

    const handleVerMisRecetas = () => router.push('/mis-recetas/propietario/propietario');
    const handleVerRecetasEnEspera = () => router.push('/en-espera-recetas');
    const handleVerIngredientesEnEspera = () => router.push('/en-espera-ingredientes');
    const handleVerIngredientes = () => router.push('/ingredientes-aprobados');
    const handleAñadirReceta = () => router.push('/anadir-receta/propietario');
    const handleCrearAyudaCabecera = () => router.push('/crearAyuda');
    const handleResponder = () => router.push('/peticiones');

    const obtenerDudasNoRespondidas = async () => {
        try {
            const response = await fetch("/routes/peticiones", {
                method: "GET",
                headers: { "Content-Type": "application/json" },
            });
            const data = await response.json();
            setDudas(data);
        } catch (error) {
            console.error("Error al obtener dudas no respondidas:", error);
        }
    };
    
    // Llama a `obtenerDudasNoRespondidas` dentro del `useEffect` inicial
    useEffect(() => {
        obtenerDudasNoRespondidas();

        // Configura el intervalo para actualizar la lista cada 5 segundos (5000 ms)
        const interval = setInterval(() => {
            obtenerDudasNoRespondidas();
        }, 5000);

        // Limpia el intervalo cuando el componente se desmonta
        return () => clearInterval(interval);
    }, []);
    
    const handleOpenPopup = (duda) => {
        setSelectedDuda(duda);
        setShowPopup(true);
    };

    const handleClosePopup = () => {
        setShowPopup(false);
        setSelectedDuda(null);
        setRespuesta("");
    };

    const handleSubmitResponse = async () => {
        if (!respuesta) return;
    
        try {
            console.log(selectedDuda.correo, selectedDuda.fecha_consulta, respuesta, nomUsuario);
            const response = await fetch('/routes/responder_consulta', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    correo: selectedDuda.correo,
                    fecha_consulta: selectedDuda.fecha_consulta,
                    respuesta,
                    nom_usuario_rc: nomUsuario,
                }),
            });
    
            if (response.ok) {
                const responseP = await fetch('/routes/peticiones', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({
                        correo: selectedDuda.correo,
                        fecha_consulta: selectedDuda.fecha_consulta,
                        respuesta,
                        nom_usuario_rc: nomUsuario,
                    }),
                });

                if (responseP.ok) {
                    console.log("Respuesta enviada con éxito");
                    alert("Respuesta enviada con éxito");
        
                    handleClosePopup();
                    // Esperar un momento antes de obtener los datos actualizados
                    // Espera 500 ms antes de actualizar
                    setTimeout(obtenerDudasNoRespondidas, 500); 
                }
                else {
                    console.error("Error al enviar la respuesta");
                    const errorData = await response.json();
                    console.error("Detalles del error:", errorData);
                }
            } else {
                console.error("Error al enviar la respuesta");
                const errorData = await response.json();
                console.error("Detalles del error:", errorData);
            }
        } catch (error) {
            console.error("Error al enviar la respuesta:", error);
        }
    };

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

            <div className="ancho-completo">
                <div className="contenedor-ayudas">
                    <h3>Dudas no respondidas:</h3>
                    {dudas.length > 0 ? (
                <div className="lista-ayudas">
                {dudas.map((duda, index) => (
                    <div key={index} className="ayuda-item">
                        <strong>{duda.asunto}</strong>
                        <p>{duda.pregunta}</p>
                        <small>{new Date(duda.fecha_consulta).toLocaleString()}</small>
                        <button 
                            onClick={() => handleOpenPopup(duda)}
                            style={{
                                backgroundColor: "#ffffff",
                                color: "#ff99cc",
                                border: "2px solid #ff99cc",
                                borderRadius: "5px",
                                padding: "8px 16px",
                                fontSize: "16px",
                                width: "100%",
                                cursor: "pointer",
                                transition: "all 0.3s ease"
                            }}
                        >
                            Responder
                        </button>
                    </div>
                ))}
            </div>
                    ) : (
                        <p>No hay dudas no respondidas.</p>
                    )}                
                    {showPopup && (
                        <div 
                            style={{
                                position: "fixed",
                                top: 0,
                                left: 0,
                                width: "100vw",
                                height: "100vh",
                                background: "rgba(0, 0, 0, 0.5)",
                                display: "flex",
                                justifyContent: "center",
                                alignItems: "center",
                                zIndex: 1000
                            }}
                            onClick={handleClosePopup}
                        >
                            <div 
                                style={{
                                    background: "#fcb9da",
                                    padding: "20px",
                                    borderRadius: "10px",
                                    width: "33vw",
                                    maxWidth: "500px",
                                    minWidth: "300px",
                                    textAlign: "center",
                                    boxShadow: "0px 4px 8px rgba(0, 0, 0, 0.3)"
                                }}
                                onClick={(e) => e.stopPropagation()}
                            >
                                <h3 style={{ fontSize: "1.5rem", margin: "10px 0" }}>Escribe tu respuesta</h3>
                                
                                {/* Detalles de la duda seleccionada */}
                                {selectedDuda && (
                                    <div style={{ 
                                        width: "100%", 
                                        textAlign: "left", 
                                        margin: "10px 0", 
                                        padding: "10px", 
                                        backgroundColor: "#ffe6f0", 
                                        borderRadius: "5px", 
                                        boxSizing: "border-box", 
                                        boxShadow: "0px 2px 4px rgba(0, 0, 0, 0.1)"
                                    }}>
                                        <p style={{ margin: "5px 0" }}><strong>Nombre:</strong> {selectedDuda.nombre}</p>
                                        <p style={{ margin: "5px 0" }}><strong>Asunto:</strong> {selectedDuda.asunto}</p>
                                        <p style={{ margin: "5px 0" }}><strong>Pregunta:</strong> {selectedDuda.pregunta}</p>
                                    </div>
                                )}


                                <textarea
                                    value={respuesta}
                                    onChange={(e) => setRespuesta(e.target.value)}
                                    placeholder="Escribe tu respuesta aquí..."
                                    style={{
                                        width: "calc(100% - 20px)",
                                        height: "120px", 
                                        margin: "10px 0",
                                        padding: "10px",
                                        fontSize: "1rem",
                                        fontFamily: "Arial, sans-serif",
                                        borderRadius: "5px",
                                        border: "1px solid #ddd",
                                        resize: "none",
                                        boxShadow: "0px 2px 4px rgba(0, 0, 0, 0.1)"
                                    }}
                                />
                                <button 
                                    onClick={handleSubmitResponse} 
                                    style={{ 
                                        backgroundColor: "#ff99cc", 
                                        color: "white", 
                                        padding: "10px 0", 
                                        fontSize: "1rem", 
                                        border: "none", 
                                        borderRadius: "5px", 
                                        cursor: "pointer", 
                                        margin: "10px 0",
                                        width: "calc(100% - 20px)", 
                                        boxShadow: "0px 2px 4px rgba(0, 0, 0, 0.2)"
                                    }}
                                >
                                    Enviar Respuesta
                                </button>
                                <button 
                                    onClick={handleClosePopup} 
                                    style={{ 
                                        backgroundColor: "#ddd", 
                                        color: "black", 
                                        padding: "10px 0", 
                                        fontSize: "1rem", 
                                        border: "none", 
                                        borderRadius: "5px", 
                                        cursor: "pointer", 
                                        margin: "10px 0",
                                        width: "calc(100% - 20px)"
                                    }}
                                >
                                    Cancelar
                                </button>
                            </div>
                        </div>
                    )}

                </div>
            </div>
        </div>
    );
}

export default Peticiones;
