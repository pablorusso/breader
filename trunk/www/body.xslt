<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:transform
	xmlns:xsl = "http://www.w3.org/1999/XSL/Transform"
	xmlns:fn  = "http://www.w3.org/2005/02/xpath-functions"
	version   = "1.0">

	<xsl:output method="html"/>

	<xsl:template match="articles">
		<div id="entries">
			<xsl:if test="count(article) = 0">
				<div id="no_art" class="entry" style="height: 100%;">
					<table class="card" style="height: 100%;">
						<tr>
							<td class="ctl"/>
							<td class="ct"/>
							<td class="ctr"/>
						</tr>
						<tr style="height: 100%;">
							<td class="cl"/>
							<td class="cc" align="center">
								<div class="entry-container entry-main">
									<div class="entry-date"/>
									<table>
										<tr>
											<td/>
											<td/>
											<td/>
											<td>
												<h2 class="entry-title">
													<span align="center">No se encontraron articulos</span>
												</h2>
											</td>

											<td/>
										</tr>
									</table>
									<div class="entry-author"/>
									<div class="entry-body"/>
								</div>
							</td>
							<td class="cr"/>
						</tr>
						<tr class="card-bottomrow">
							<td class="cbl"/>
							<td class="cb"/>
							<td class="cbr"/>
						</tr>
					</table>
				</div>
			</xsl:if>

			<xsl:apply-templates select="article"/>
		</div>
	</xsl:template>

	<xsl:template match="article">
		<div class="entry">
			<table class="card">
				<tr>
					<td class="ctl"/>
					<td class="ct"/>
					<td class="ctr"/>
				</tr>
				<tr>
					<td class="cl"/>

					<td class="cc">
						<div class="entry-container entry-main">
							<!--fecha-->
							<xsl:variable name="timestamp">
								<xsl:value-of select="concat('http://localhost/breader/convert.php?timestamp=', @date)"/>
							</xsl:variable>
							<div class="entry-date">
								<xsl:value-of select="document( $timestamp )/formatted" />
							</div>

							<!--titulo y link-->
							<table>
							<tr>
								<td>
									<xsl:if test="@isFavourite != 0">
										<img style="border:0" src="images/status_fav.png" title="Favorito" alt="Favorito"/>
									</xsl:if>
								</td>
								<td>
									<xsl:if test="@read = 0">
										<img style="border:0" src="images/status_unread.png" title="No leido" alt="No leido"/>
									</xsl:if>
								</td>
								<td>
									<xsl:if test="@isClassified = 0">
										<img style="border:0" src="images/status_unclasi.png" title="No clasificado" alt="No clasificado"/>
									</xsl:if>
								</td>
								<td>
									<h2 class="entry-title">
										<a target="_blank" class="entry-title-link">
											<xsl:attribute name="href">
												<xsl:value-of select="@link"/>
											</xsl:attribute>
											<xsl:value-of select="@title"/>
										</a>
									</h2>
								</td>

								<td>
									<a target="_blank" class="entry-title-link">
										<xsl:attribute name="href">
											<xsl:value-of select="@link"/>
										</xsl:attribute>
										<img style="border:0" class="entry-title-go-to" src="images/action_go_to.png" title="Ir a la pagina de la noticia" alt="Ir a la pagina de la noticia"/>
									</a>
								</td>
							</tr>
							</table>


							<!--autor-->
							<div class="entry-author">
								publicado por <xsl:value-of select="@author"/> en <xsl:value-of select="@feedName"/>
							</div>

							<!--resumen-->
							<div id="summary" class="entry-body">
								<xsl:value-of select="summary" disable-output-encoding="yes"/>
							</div>
						</div>
					</td>

					<td class="cr"/>
				</tr>

				<tr class="card-actionrow">
					<td class="cal"/>

					<td class="ca">
						<div class="entry-actions">
							<table>
								<tr align="left">
									<td align="left">
										<span class="star link">
											<xsl:attribute name="onclick">
												doAction( 'actionCode=' + escape( 'A8' ) + '&amp;params=' + escape( 'feedId||#<xsl:value-of select="@feedId"/>|||artId||#<xsl:value-of select="@artId"/>' ), editArticleHandler, '' )
											</xsl:attribute>
											<xsl:choose>
												<xsl:when test="@isFavourite = 0">
													<xsl:text>Agregar a favoritos</xsl:text>
												</xsl:when>
												<xsl:otherwise>
													<xsl:text>Quitar de favoritos</xsl:text>
												</xsl:otherwise>
											</xsl:choose>
										</span>
									</td>
									<td align="left">
										<span class="star link">
											<xsl:attribute name="onclick">
												doAction( 'actionCode=' + escape( 'A9' ) + '&amp;params=' + escape( 'feedId||#<xsl:value-of select="@feedId"/>|||artId||#<xsl:value-of select="@artId"/>' ), editArticleHandler, '')
											</xsl:attribute>
											<xsl:choose>
												<xsl:when test="@read = 0">
													<xsl:text>Marcar como leido</xsl:text>
												</xsl:when>
												<xsl:otherwise>
													<xsl:text>Marcar como no leido</xsl:text>
												</xsl:otherwise>
											</xsl:choose>
										</span>
									</td>
									<td>
										<a class="link">
											<xsl:attribute name="onclick">
												showCatsMenu( '<xsl:value-of select="@feedId"/>', '<xsl:value-of select="@artId"/>', '<xsl:value-of select="@id"/>' );
											</xsl:attribute>
											<xsl:attribute name="onmouseout">
												startTimerToSelectCat();
											</xsl:attribute>
											<img style="border:0" src="images/action_tag_add.png" title="Asociar categoría" alt="Asociar categoría"/>
										</a>
									</td>
									<td class="star">
											Categorías :
									</td>
									<td>
										<xsl:apply-templates select="tags"/>
									</td>
								</tr>
							</table>
						</div>
					</td>

					<td class="car"/>
				</tr>

				<tr class="card-bottomrow">
					<td class="cbl"/>
					<td class="cb"/>
					<td class="cbr"/>
				</tr>

			</table>
		</div>
	</xsl:template>

	<xsl:template match="tags">
		<table>
			<tr>
				<xsl:for-each select="tag">
					<xsl:if test="@readOnly = 0">
						<xsl:if test="position() &gt; 1">
							<xsl:text>, </xsl:text>
						</xsl:if>

						<xsl:if test="@isApproved = 0">
							<td>
							<a class="link">
								<xsl:attribute name="onclick">
								doAction( 'actionCode=' + escape('A3') + '&amp;params=' + escape( 'feedId||#<xsl:value-of select="@feedId"/>|||artId||#<xsl:value-of select="../../@artId"/>|||tagId||#<xsl:value-of select="@id"/>' ), editArticleHandler, '')
								</xsl:attribute>
								<img style="border:0" src="images/action_tag_check.png" title="Aprobar clasificacion automatica" alt="Aprobar clasificacion automatica"/>
							</a>
							</td>
						</xsl:if>

						<td>
						<a class="link">
							<xsl:attribute name="onclick">
								doAction( 'actionCode=' + escape( 'A2' ) + '&amp;params=' + escape( 'feedId||#<xsl:value-of select="@feedId"/>|||artId||#<xsl:value-of select="../../@artId"/>|||tagId||#<xsl:value-of select="@id"/>' ), editArticleHandler, '')
							</xsl:attribute>
							<img style="border:0" src="images/action_tag_del.png" title="Desasociar categoría" alt="Desasociar categoría"/>
						</a>
						</td>
						<td class="star">
							<xsl:text> </xsl:text>
							<xsl:value-of select="@name"/>
						</td>
					</xsl:if>
				</xsl:for-each>
			</tr>
		</table>
	</xsl:template>

</xsl:transform>