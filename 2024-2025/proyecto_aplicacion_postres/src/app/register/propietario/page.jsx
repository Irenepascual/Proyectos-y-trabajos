"use client";
import { useForm } from "react-hook-form";
import { useRouter } from "next/navigation";
import { useState } from "react";

import "/src/app/style-register-login.css";

// crea una pagina de registro de propietario que maneja los datos del formulario, 
// valida las entradas del usuario, y envia los datos al servidor para crear un nuevo propietario, 
// con manejo de errores tanto en el cliente como en la respuesta del servidor.


function RegisterPropietarioPage() {

    const { register, handleSubmit, formState: {errors}, } = useForm();
    const router = useRouter();

    const [serverError, setServerError] = useState("");

    const onSubmit = handleSubmit(async data => {
        try{
            const res = await fetch('/routes/registrarPropietario', {
                method: "POST",
                body: JSON.stringify({
                    nom_usuario: data.nom_usuario,
                    codigo: data.codigo,
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
                setServerError(errorData.message || "Error al registrar el propietario");
                return;
            }
            
            router.push('/login/propietario');
            router.refresh();
            
        } catch(error) {
            setServerError("Error al procesar la solicitud");
        }
    });

    console.log(errors);

    return (
        <div className="login-page">
            <form onSubmit={onSubmit} className="login-form">
                <h1>Registrar Propietario</h1>

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

                <label>Código:
                    <input 
                        type="number"  
                        {...register("codigo", {
                            required: {
                                value: true,
                                message: "El código es obligatorio",
                            },
                        })}
                        placeholder="Código de propietario"
                    />
                </label>
                {errors.codigo && (
                    <span className="error-text">
                        {errors.codigo.message}
                        <br />
                    </span>
                )}

                <button className="login-button" type="submit">Registrar</button>
            </form>
        </div>
    );
}

export default RegisterPropietarioPage;

