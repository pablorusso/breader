<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

	<xsl:output method="html"/>

	<xsl:template match="articles">
		<table>
			<tr>
				<td>
					<div id="entries" >
						<xsl:apply-templates select="article"/>
					</div>
				</td>
			</tr>
		</table>
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
							<div class="entry-date">
								<xsl:value-of select="@date"/>
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
								publicado por <xsl:value-of select="@author"/> en <xsl:value-of select="@feed"/>
							</div>

							<!--resumen-->
							<div class="entry-body">
								<xsl:value-of select="summary"/>
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
												doAction( '?actionCode=A8&amp;params=artId%23<xsl:value-of select="@id"/>', editArticleHandler, '' )
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
												doAction('?actionCode=A9&amp;params=artId%23<xsl:value-of select="@id"/>', editArticleHandler, '')
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
									<td class="star">
											Categorias (
									</td>
									<td>
										<a class="link">
											<xsl:attribute name="onclick">
												addTag( '<xsl:value-of select="@id"/>' )
											</xsl:attribute>
											<img style="border:0" src="images/action_tag_add.png" title="Agregar categoria" alt="Agregar categoria"/>
										</a>
									</td>
									<td class="star">
											):
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
					<td class="catName">
						<xsl:value-of select="@name"/>
					</td>
					<xsl:if test="@isApproved = 0">
						<td>
						<a class="link">
							<xsl:attribute name="onclick">
							doAction('?actionCode=A3&amp;params=artId%23<xsl:value-of select="../../@id"/>%7CtagId%23<xsl:value-of select="@id"/>', editArticleHandler, '')
							</xsl:attribute>
							<img style="border:0" src="images/action_tag_check.png" title="Aprobar clasificacion automatica" alt="Aprobar clasificacion automatica"/>
						</a>
						</td>
					</xsl:if>

					<td>
					<a class="link">
						<xsl:attribute name="onclick">
							doAction('?actionCode=A2&amp;params=artId%23<xsl:value-of select="../../@id"/>%7CtagId%23<xsl:value-of select="@id"/>', editArticleHandler, '')
						</xsl:attribute>
						<img style="border:0" src="images/action_tag_del.png" title="Borrar categoria" alt="Borrar categoria"/>
					</a>
					</td>
				</xsl:for-each>
			</tr>
		</table>
	</xsl:template>

</xsl:transform>