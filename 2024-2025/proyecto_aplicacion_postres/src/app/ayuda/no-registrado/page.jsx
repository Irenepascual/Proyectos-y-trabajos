"use client";
import React from 'react';
import HeaderSinSesion from "@/app/componentes/HeaderSinSesion";
import Ayuda from "@/app/componentes/ayuda";

/*
* Devuelve la página de ayuda, donde se muestra
* la cabecera de usuario no registrado y la 
* información sobre todas las ayudas disponibles
*/
export default function AyudaNoUsuario() {
    return (
        <div>
            <HeaderSinSesion />
            <Ayuda />
        </div>
    );
}
