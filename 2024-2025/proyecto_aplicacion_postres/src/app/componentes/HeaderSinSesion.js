import "/src/app/style-header.css";
import React from 'react';
import Link from 'next/link';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faUser, faCircleQuestion, faAddressCard } from '@fortawesome/free-solid-svg-icons';

/*
* Devuelve la cabecera para usuarios no registrados, donde se
* muestran las ayudas, el contacto y la posibilidad de poder
* iniciar sesión, tanto como usuario como propietario. 
*/
export default function HeaderSinSesion() {
    return (
        <div className="all">
            <div>
                <title>POSTRECICOS</title>
            </div>
            <div>            
            <h1>
                <Link href='/' passHref>
                    <span className="logo">POSTRECICOS</span> 
                </Link>
            </h1>
                <div>
                    <ul className="lista-horizontal">
                        <li>
                            <FontAwesomeIcon icon={faAddressCard} style={{color: "#feecef",}} className="icono-cabecera"/>
                            <a href="/contacto/no-registrado">Contacto</a>
                        </li>
                        <li>
                            <FontAwesomeIcon icon={faCircleQuestion} style={{color: "#feecef",}} className="icono-cabecera"/>
                            <a href="/ayuda/no-registrado">Ayuda</a>
                        </li>
                        <li className="dropdown">
                            <FontAwesomeIcon icon={faUser} style={{color: "#feecef",}} className="icono-cabecera"/>
                            <a href="#">Iniciar Sesión</a>
                            <ul className="dropdown-content">
                                <li><a href="/login/usuario">Usuario</a></li>
                                <li><a href="/login/propietario">Propietario</a></li>
                            </ul>
                        </li>
                    </ul>
                </div>
            </div>
        </div>
    );
}