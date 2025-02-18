"use client";
import { useForm } from "react-hook-form";
import { useRouter } from "next/navigation";

import { useState } from "react";

import "/src/app/style-register-login.css";

// Crea una pagina de registro para usuarios que maneja el estado de los datos del formulario, 
// realiza la validacion de los campos de entrada, envia los datos al servidor para registrar al usuario, 
// y maneja los posibles errores tanto del servidor como de la validacion del formulario. Ademas, 
// redirige al usuario a la pagina de login si el registro es exitoso.

function RegisterUsuarioPage() {

    const { register, handleSubmit, formState: {errors}, } = useForm();
    const router = useRouter();

    const [serverError, setServerError] = useState("");


    const onSubmit = handleSubmit(async data => {
        try {
            console.log("Los datos", data);
            const res = await fetch('/routes/registrarUsuario', {
                method: "POST",
                body: JSON.stringify({
                    nom_usuario: data.nom_usuario,
                    correo: data.correo,
                    contrasena: data.contrasena,
                }),
                headers: {
                    "Content-Type": "application/json",
                },
            });
            console.log(res);

            if (!res.ok) {
                const errorData = await res.json();
                setServerError(errorData.message || "Error al registrar el usuario");
                return;
            }
            if (res.ok) {
                router.push("/login/usuario");
                router.refresh();
            }

        } catch(error) {
            setServerError("Error al procesar la solicitud");
        }
        
    });

    console.log(errors);

return (
    <div className="login-page">
        <form onSubmit={onSubmit} className="login-form">
            <h1>Registrar Usuario</h1>

            {serverError && (
                <p className="error-message">
                    {serverError}
                </p>
            )}

            <label>Nombre de usuario:
                <input 
                    type="text"
                    {...register("nom_usuario", {
                        required: {
                            value: true,
                            message: "El nombre de usuario es obligatorio",
                        },
                    })}
                    placeholder="miUsuario123"
                />
            </label>
            {errors.nom_usuario && (
                <span className="error-text">
                    {errors.nom_usuario.message}
                    <br />
                </span>
            )}

            <label>Correo electrónico:
                <input 
                    type="email"
                    {...register("correo", {
                        required: {
                            value: true,
                            message: "El correo es obligatorio",
                        },
                    })}
                    placeholder="usuario@gmail.com"
                />
            </label>
            {errors.correo && (
                <span className="error-text">
                    {errors.correo.message}
                    <br />
                </span>
            )}

            <label>Contraseña:
                <input
                    type="password"
                    {...register("contrasena", {
                        required: {
                            value: true,
                            message: "La contraseña es obligatoria",
                        },
                    })}
                    placeholder="******"
                />
            </label>
            {errors.contrasena && (
                <span className="error-text">
                    {errors.contrasena.message}
                    <br />
                </span>
            )}

            <button className="login-button" type="submit">Registrar</button>
        </form>
    </div>
);
}


export default RegisterUsuarioPage;

