"use client";
import "/src/app/style-header.css";
import Link from 'next/link';
import React, { useState, useEffect } from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faUser, faCircleQuestion, faAddressCard, faHeart, faUtensils, faPlus } from '@fortawesome/free-solid-svg-icons';

/*
* Devuelve la cabecera para usuarios registrados, donde se
* muestran los favoritos, las recetas creadas por el usuario, 
* la posibilidad de añadir recetas, la posibilidad de añadir
* ingredientes, el contacto, la ayuda y el nombre del usuario
* (junto con las posibilidades de poder cerrar sesión o de 
* eliminar cuenta). 
* El nombre del usuario lo recoge del almacenamiento local, y las
* que debe tomar en caso de que se pulse algún botón de la 
* cabecera, se le explica en otros archivo (que le pasan las
* funciones handle).
*/
function HeaderConSesion({ handleLogout, handleVerMisFavoritos, handleVerMisRecetas, handleAñadirReceta, handleAñadirIngrediente, handleContacto, handleAyuda }) {
    const [nomUsuario, setNomUsuario] = useState('');
    const [showConfirmDelete, setShowConfirmDelete] = useState(false); // Aquí se define correctamente la variable
    
    useEffect(() => {
        // Solo ejecutar en el cliente
        const storedNomUsuario = localStorage.getItem('nom_usuario');
        setNomUsuario(storedNomUsuario || 'Usuario');
        console.log("NOMBRE DE USUARIO:", storedNomUsuario);
    }, []);

    const handleEliminarCuenta = async () => {
        const storedNomUsuario = localStorage.getItem('nom_usuario');
    
        // Verificar si hay un usuario en localStorage
        if (!storedNomUsuario) {
            alert("No se encontró el usuario en localStorage");
            return;
        }
       
        try {
            const response = await fetch('/routes/eliminar-usuario', {
                method: 'DELETE',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ nom_usuario: storedNomUsuario })
            });
    
            if (response.ok) {
                alert("Cuenta eliminada correctamente");
    
                // Limpiar el localStorage y redirigir a la página sin sesión
                localStorage.removeItem('nom_usuario');
                
                // Prevenir que la página actual realice más acciones
                setTimeout(() => {
                    window.location.href = '';
                }, 500);
            } else {
                const errorData = await response.json();
                console.error("Error al eliminar la cuenta:", errorData);
                alert("Error al eliminar la cuenta");
            }
        } catch (error) {
            console.error("Error al eliminar la cuenta:", error);
            alert("Error al eliminar la cuenta");
        }
    };
    
 
    
    return (
        <div>
            <title>POSTRECICOS</title>
            <h1>
                <Link href="/home-con-sesion" passHref>
                    <span className="logo">POSTRECICOS</span> {/* Aplica la clase `logo` */}
                </Link>
            </h1>
            <ul className="lista-horizontal">
                <li>
                    <FontAwesomeIcon icon={faAddressCard} style={{ color: "#feecef" }} className="icono-cabecera" />
                    <a onClick={handleContacto} className="clickable">Contacto</a>
                </li>                
                <li>
                    <FontAwesomeIcon icon={faCircleQuestion} style={{ color: "#feecef" }} className="icono-cabecera" />
                    <a onClick={handleAyuda} className="clickable">Ayuda</a>
                </li>
                <li>
                    <FontAwesomeIcon icon={faHeart} style={{ color: "#feecef" }} className="icono-cabecera" />
                    <a onClick={handleVerMisFavoritos} className="clickable">Mis favoritos</a>
                </li>
                <li>
                    <FontAwesomeIcon icon={faUtensils} style={{ color: "#feecef" }} className="icono-cabecera" />
                    <a onClick={handleVerMisRecetas} className="clickable">Mis recetas</a>
                </li>
                <li>
                    <FontAwesomeIcon icon={faPlus} style={{ color: "#feecef" }} className="icono-cabecera" />
                    <a onClick={handleAñadirReceta} className="clickable">Añadir receta</a>
                </li>
                <li>
                    <FontAwesomeIcon icon={faPlus} style={{ color: "#feecef" }} className="icono-cabecera" />
                    <a onClick={handleAñadirIngrediente} className="clickable">Añadir ingrediente</a>
                </li>

                <li className="dropdown">
                    <FontAwesomeIcon icon={faUser} style={{ color: "#feecef" }} className="icono-cabecera" />
                    <a href="#">{nomUsuario}</a>
                    <ul className="dropdown-content">
                        <li><a onClick={handleLogout} className="clickable">Cerrar sesión</a></li>
                        <li><a onClick={() => setShowConfirmDelete(true)} className="clickable eliminar-cuenta">Eliminar cuenta</a></li>
                    </ul>
                </li>
            </ul>

            {showConfirmDelete && (
                <div className="confirm-delete-overlay">
                    <div className="confirm-delete-box">
                        <p>¿Está seguro que desea eliminar la cuenta?</p>
                        <button onClick={handleEliminarCuenta} className="btn-confirm">Sí</button>
                        <button onClick={() => setShowConfirmDelete(false)} className="btn-cancel">No</button>
                    </div>
                </div>
            )}
        </div> 
    );
}

export default HeaderConSesion;
