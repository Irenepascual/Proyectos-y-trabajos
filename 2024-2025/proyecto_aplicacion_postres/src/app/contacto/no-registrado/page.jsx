"use client";
import React from 'react';
import HeaderSinSesion from "@/app/componentes/HeaderSinSesion";
import Contacto from "@/app/componentes/contacto";

/*
* Devuelve la página de contacto, donde se muestra
* la cabecera de usuario no registrado y un cuestionario 
* para completar con las dudas que se tienen. 
*/
export default function ContactoNoUsuario() {
    return (
        <div>
            <HeaderSinSesion /> 
            <Contacto />
        </div>
    );
}
