"use client";
import React, { useEffect, useState } from 'react';
import { useRouter } from "next/navigation";
import HeaderConSesion from '/src/app/componentes/HeaderConSesion';
import '/src/app/style-anadir-ingrediente.css';

/*
* Devuelve la página de añadir ingrediente, donde se muestra
* la cabecera de usuario registrado y permite añadir un ingrediente
* con su nombre y las posibles restricciones que puede tener
*/
export default function AnadirIngredienteUsuario() {
    const [nomIngrediente, setNomIngrediente] = useState('');
    const [mensaje, setMensaje] = useState('');
    const [restricciones, setRestricciones] = useState([]);
    const [seleccionadas, setSeleccionadas] = useState([]);
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

    useEffect(() => {
        const fetchRestricciones = async () => {
            try {
                const response = await fetch('/routes/restricciones');
                const data = await response.json();
                setRestricciones(data);
            } catch (error) {
                console.error("Error al obtener restricciones:", error);
            }
        };
        fetchRestricciones();
    }, []);

    const handleLogout = () => {
        localStorage.removeItem('nom_usuario');
        setNomUsuario('');
        router.push('/');
    };

    const handleContacto = () => router.push('/contacto/usuario');
    const handleAyuda = () => router.push('/ayuda/usuario');
    const handleVerMisRecetas = () => router.push('/mis-recetas/usuario');
    const handleVerMisFavoritos = () => router.push('/mis-favoritos');
    const handleAñadirReceta = () => router.push('/anadir-receta/usuario');
    const handleAñadirIngrediente = () => router.push('/anadir-ingrediente/usuario')

    const addIngrediente = async () => {
        const storedNomUsuario = localStorage.getItem('nom_usuario');
        if (!nomIngrediente || !storedNomUsuario) {
            setMensaje("El nombre del ingrediente no puede estar vacío");
            return;
        }
    
        try {
            const response = await fetch('/routes/ingredientes', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    nom_usuario: storedNomUsuario,
                    nom_ingred: nomIngrediente,
                    restricciones: seleccionadas,
                    estado: "no_aprobado",
                })
            });
    
            if (response.ok) {
                const resp = await fetch('/routes/tiene_restriccion', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify({
                        nom_usuario_t: storedNomUsuario,
                        nom_ingred_t: nomIngrediente,
                        restricciones: seleccionadas,
                    })
                });

                setMensaje("Ingrediente añadido con éxito a pendientes");
                setNomIngrediente(''); 
                setSeleccionadas([]);

                setTimeout(() => {
                    router.push('/home-con-sesion');
                }, 2500); 
            } else {
                setMensaje("Error al añadir ingrediente");
            }
        } catch (error) {
            console.error("Error al añadir ingrediente:", error);
            setMensaje("Error al añadir ingrediente");
        }
    };
    
    const toggleRestriccion = (nom_restr) => {
        setSeleccionadas((prev) =>
            prev.includes(nom_restr) ? prev.filter((res) => res !== nom_restr) : [...prev, nom_restr]
        );
    };

    return (
        <div>
            <HeaderConSesion
                handleLogout={handleLogout}
                handleVerMisFavoritos={handleVerMisFavoritos}
                handleVerMisRecetas={handleVerMisRecetas}
                handleAñadirReceta={handleAñadirReceta}
                handleAñadirIngrediente={handleAñadirIngrediente}
                handleContacto={handleContacto}
                handleAyuda={handleAyuda}
            />
            <p className="titulo-anadir-ingrediente">AÑADIR INGREDIENTE</p>
            <div className="input-container">
                <input
                    type="text"
                    className="input-field"
                    value={nomIngrediente}
                    onChange={(e) => setNomIngrediente(e.target.value)}
                    placeholder="Introduce el nombre del ingrediente"
                />
            </div>

            <div className="restricciones-container">
                <h3>Marque si contiene:</h3>
                {restricciones.map((res) => (
                    <label key={res.nom_restr}>
                        <input
                            type="checkbox"
                            checked={seleccionadas.includes(res.nom_restr)}
                            onChange={() => toggleRestriccion(res.nom_restr)}
                        />
                        {res.nom_restr}
                    </label>
                ))}
            </div>


            <button onClick={addIngrediente} className="btn">Confirmar</button>
            {mensaje && (
                <p className={`mensaje ${mensaje.includes("Error") ? 'error' : 'exito'}`}>{mensaje}</p>
            )}
        </div>
    );
}

