"use client";
import React, { useState, useEffect } from 'react';
import "/src/app/style-ayuda.css";

/*
* Devuelve la lista con todas las ayudas creadas.
*/
function AyudaList({ ayudas }) {
    return (
        <div className="lista-ayudas">
            {ayudas.map((ayuda, index) => (
                <div key={index} className="ayuda-item">
                    <strong>{ayuda.nom_usuario_a}:</strong> {ayuda.pregunta}
                </div>
            ))}
        </div>
    );
}

/*
* Devuelve un contenedor con todas las ayudas disponibles
*/
function Ayuda() {
    const [ayudas, setAyudas] = useState([]);
    const [isLoading, setIsLoading] = useState(true);

    useEffect(() => {
        const obtenerAyudas = async () => {
            try {
                const response = await fetch('/routes/ayuda');
                const data = await response.json();
                setAyudas(data);
            } catch (error) {
                console.error("Error al obtener ayudas:", error);
            } finally {
                setIsLoading(false); // Cambia isLoading a false cuando termina la carga
            }
        };

        obtenerAyudas();
    }, []);

    return (
        <div className="contenedor-ayudas">
            <h3>Listado de Ayudas</h3>
            {isLoading ? (
                <div className="loading-container">
                    <div className="spinner"></div> 
                </div>
            ) : ayudas.length > 0 ? (
                <AyudaList ayudas={ayudas} />
            ) : (
                <p>No hay ayudas disponibles.</p>
            )}
        </div>
    );
}

export default Ayuda;
