"use client";
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import HeaderConSesionP from "/src/app/componentes/HeaderConSesionP";
import "/src/app/style-header.css";
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
* Devuelve la página de crear ayuda para propietario, donde 
* se muestra la cabecera del propietario, todas las ayudas 
* que hay actualmente, una opción para permitir añadir más
* ayudas y una opción de eliminar la ayuda que se seleccione.
* Cada cierto tiempo se recargan automáticamente las ayudas 
* disponibles.
*/
function AyudaConSesion() {
    const router = useRouter();
    const [ayudas, setAyudas] = useState([]);  // Estado para almacenar las ayudas
    const [pregunta, setPregunta] = useState('');  // Estado para almacenar la pregunta de la ayuda
    const [nomUsuario, setNomUsuario] = useState('');  // Estado para almacenar el nombre de usuario

    // Obtener el nombre de usuario del localStorage y todas las ayudas desde la API
    useEffect(() => {
        const storedNomUsuario = localStorage.getItem('nom_usuario');
        if (storedNomUsuario) {
            setNomUsuario(storedNomUsuario);
        } else {
            router.push('/'); // Redirige si no hay un usuario autenticado
        }

        obtenerAyudas(); // Cargar las ayudas al montar el componente
    }, [router]);

    useEffect(() => {
        const fetchAyudasActualizar = async () => {
            try {
                const response = await fetch('/crearAyuda', { method: 'GET' });
                if (!response.ok) throw new Error("Error al obtener las ayudas");
                const data = await response.json();
                setAyudas(data);
            } catch (error) {
                console.error("Error al obtener las ayudas:", error);
            }
        };
    
        // Llamar a la función al cargar la página
        fetchAyudasActualizar();
    
        // Configurar un intervalo para actualizar cada 10 segundos
        const intervalId = setInterval(fetchAyudasActualizar, 10000);
    
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
    const handleResponder = () => router.push('/peticiones');

    // Función para obtener todas las ayudas desde la API
    const obtenerAyudas = async () => {
        try {
            const response = await fetch('/routes/ayuda', {
                method: 'GET',
                headers: { 'Content-Type': 'application/json' }
            });
            const data = await response.json();
            setAyudas(data);  
        } catch (error) {
            console.error("Error al obtener ayudas: ", error);
        }
    };

    // Función para manejar la creación de una nueva ayuda
    const handleCrearAyuda = async (e) => {
        e.preventDefault();

        if (!pregunta) return;

        try {
            const response = await fetch('/routes/ayuda', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ nom_usuario_a: nomUsuario, pregunta }),
            });

            if (response.ok) {
                setPregunta(''); // Limpiar el campo de la pregunta
                obtenerAyudas(); // Refrescar la lista de ayudas
            } else {
                console.error("Error al crear ayuda: ", await response.json());
            }
        } catch (error) {
            console.error("Error al crear ayuda: ", error);
        }
    };

    // Función para manejar la eliminación de una ayuda
const handleEliminarAyuda = async (nom_usuario_a, pregunta) => {
    try {
        const response = await fetch('/routes/ayuda', {
            method: 'DELETE',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ nom_usuario_a, pregunta })
        });

        if (response.ok) {
            console.log("Ayuda eliminada con éxito");
            obtenerAyudas(); // Refrescar la lista de ayudas después de eliminar
        } else {
            console.error("Error al eliminar la ayuda");
        }
    } catch (error) {
        console.error("Error al eliminar la ayuda:", error);
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

            <div className="contenedor-ayudas">
                <h3>Crear Nueva Ayuda</h3>
                <form onSubmit={handleCrearAyuda}>
                    <textarea
                        value={pregunta}
                        onChange={(e) => setPregunta(e.target.value)}
                        placeholder="Escribe tu ayuda aquí"
                        rows="4"
                        cols="50"
                    />
                    <button type="submit">Crear Ayuda</button>
                </form>
                <h3>Todas las Ayudas</h3>
                    <div className="lista-ayudas">
                        {ayudas.map((ayuda, index) => (
                            <div key={index} className="ayuda-item">
                                <strong>{ayuda.nom_usuario_a}:</strong> {ayuda.pregunta}
                                <br />
                                <button onClick={() => handleEliminarAyuda(ayuda.nom_usuario_a, ayuda.pregunta)} className="btn-eliminar">
                                    Eliminar
                                </button>
                            </div>
                        ))}
                    </div>
            </div>
        </div>
    );
}

export default AyudaConSesion;
